#include "edit_chunk.h"
#include <iostream>

#include <math/math_tools.h>

namespace cell {

    using namespace undicht::tools;


    EditChunk::EditChunk() {

    }

    EditChunk::~EditChunk() {

    }


    ////////////////////////////////////////// internal functions //////////////////////////////////////////


    void EditChunk::clearStencil() {
        // clears the stencil buffer ( + if necessary, also sets the size to 256 * 256 * 256)

        // resizing will clear the buffers,
        // but will only allocate new memory if necessary

        m_stencil.resize(256);
        for(auto& v : m_stencil) {
            v.resize(256);
            for(auto& vv : v) vv.resize(256);
        }
    }


    void EditChunk::clearRefs() {

        // resizing will clear the buffers,
        // but will only allocate new memory if necessary
        m_refs.resize(256);
        for(auto& v : m_refs) {
            v.resize(256);
            for(auto& vv : v) vv.resize(256);
        }
    }


    void EditChunk::loadDrawChunk(DrawChunk* c) {
        // initializes the m_refs buffer to the DrawBuffer
        // only will do anything if the DrawBuffer is different to m_draw_chunk

        if(m_draw_chunk == c) return;

        m_draw_chunk = c;

        clearRefs(); // resizes

        for(int i = 0; i < c->m_cells.size(); i++) {

            const Cell& cell = c->m_cells[i];

            updateRefs(cell, i);
        }

    }

    void EditChunk::updateRefs(const Cell& c, int ref_id) {
        // updates the refs inside the cells volume to ref_id

        for(int x = c.pos[0]; x < c.pos[0] + c.siz[0] + 1; x++) {
            for(int y = c.pos[1]; y < c.pos[1] + c.siz[1] + 1; y++) {
                for(int z = c.pos[2]; z < c.pos[2] + c.siz[2] + 1; z++) {
                    // storing a reference to the cell

                    m_refs[x][y][z] = ref_id;
                }
            }
        }

    }


    //////////////////////////////////////////// internal functions to modify cells ////////////////////////////////////////////

    void EditChunk::getCellsSharingVolume(const Cell& c, std::vector<Cell*>& loadTo, std::vector<Cell>& shared_volume) {
        /** finds every cell of the current DrawChunk that shares some volume with c
        * @param shared_volume : the volume shared between each cell and c */

        // there is two ways to do this:
        // testing for every reference inside the Cell c
        // or testing every Cell in m_draw_chunk

        if(m_draw_chunk->m_cells.size() > (c.siz[0] + 1) * (c.siz[1] + 1) * (c.siz[2] + 1)) {
            // there is more cells in the draw chunk then reference within c

            for(int x = c.pos[0]; x < c.pos[0] + c.siz[0] + 1; x++) {
                for(int y = c.pos[1]; y < c.pos[1] + c.siz[1] + 1; y++) {
                    for(int z = c.pos[2]; z < c.pos[2] + c.siz[2] + 1; z++) {
                        // getting the cell at the position x,y,z

                        Cell* canditate = &getCell(x,y,z, m_draw_chunk);

                        // testing if it was already found
                        bool new_cell = true;

                        for(Cell* old : loadTo) {

                            if(canditate == old) {

                                new_cell = false;
                                break;
                            }
                        }

                        // adding the cell to the overlapping volumes
                        if(new_cell) {

                            loadTo.push_back(canditate);
                            shared_volume.push_back(getSharedVolume(canditate, &c));

                        }

                    }
                }
            }

        } else {
            // its faster to just test every cell in m_draw_chunk

            for(Cell& candidate : m_draw_chunk->m_cells) {

                if(cellsSharingVolume(&candidate, &c)) {

                    loadTo.push_back(&candidate);
                    shared_volume.push_back(getSharedVolume(&candidate, &c));
                }

            }

        }

    }


    Cell EditChunk::getSharedVolume(const Cell* c1, const Cell* c2) {
        /** @return a cell representing the overlapping volume between the two cells
        * the material of the returned cell will be equal to c2s material */

        unsigned int x1 = c1->pos[0];
        unsigned int y1 = c1->pos[1];
        unsigned int z1 = c1->pos[2];

        unsigned int x2 = c2->pos[0];
        unsigned int y2 = c2->pos[1];
        unsigned int z2 = c2->pos[2];

        unsigned int x = std::max(x1, x2);
        unsigned int y = std::max(y1, y2);
        unsigned int z = std::max(z1, z2);

        unsigned int sx = std::min((x1 + c1->siz[0] - x), (x2 + c2->siz[0] - x));
        unsigned int sy = std::min((y1 + c1->siz[1] - y), (y2 + c2->siz[1] - y));
        unsigned int sz = std::min((z1 + c1->siz[2] - z), (z2 + c2->siz[2] - z));

        return Cell({x,y,z, sx, sy, sz, c2->mat});
    }

    bool EditChunk::cellsSharingVolume(const Cell* c1, const Cell* c2) {
        /** @return true, if the cells volumes intersect at some point */

        for(int i = 0; i < 3; i++) {
            // testing for every dimension

            if(!overlappingRanges<int>(c1->pos[i], c1->pos[i] + c1->siz[i], c2->pos[i], c2->pos[i] + c2->siz[i])) {

                return false;
            }
        }

        return true;
    }


    unsigned int EditChunk::editCell(Cell* old, Cell* edit, std::vector<Cell>& new_cells) {
        /** @brief calculates the changes needed to set "edit" into the old cell
        * if old and edit share the same material, nothing happens
        * if old and edit share the same volume, olds material will be set to edits
        * if edit is part of old,
        *   + new cells will be created to take up the volume difference between old and edit (they will have old material)
        *   + the number of new cells will be returned
        *   + old will be replaced with edit */

        int new_cell_count = 0;

        if(old->mat == edit->mat) {
            // case 1: old and edit have the same material

            return 0;
        } else if((old->pos[0] == edit->pos[0]) && (old->pos[1] == edit->pos[1]) && (old->pos[2] == edit->pos[2])
                  && (old->siz[0] == edit->siz[0]) && (old->siz[1] == edit->siz[1]) && (old->siz[2] == edit->siz[2])) {
            // case 2: old and edit have the same volume

            old->mat = edit->mat;

            return 0;
        } else {
            // case 3: assuming edits volume is a part of old
            // creating a maximum of 6 new cells, depending on if edit touches one or more sides of old
            // this could be the only case, since it handles the other two cases as well ...

            int oxtoex = edit->pos[0] - old->pos[0]; // olds x to edits x
            int oytoey = edit->pos[1] - old->pos[1]; // olds y to edits y
            int oztoez = edit->pos[2] - old->pos[2]; // olds z to edits z

            int efxtoofx = old->siz[0] - edit->siz[0] - oxtoex; // edits final x to olds final x
            int efytoofy = old->siz[1] - edit->siz[1] - oytoey; // distance between the other edges
            int efztoofz = old->siz[2] - edit->siz[2] - oztoez;

            if(oytoey > 0) {
                // creating a cell below edit (full x + z size)

                new_cell_count++;

                new_cells.push_back({ old->pos[0], old->pos[1], old->pos[2],
                                      old->siz[0], oytoey - 1, old->siz[2],
                                      old->mat});
            }

            if(efytoofy > 0) {
                // creating a cell on top of edit (full x + z size)

                new_cell_count++;

                new_cells.push_back({old->pos[0], edit->pos[1] + edit->siz[1] + 1, old->pos[2],
                                     old->siz[0], efytoofy - 1, old->siz[2],
                                     old->mat});
            }

            if(oztoez > 0) {
                // creating a cell behind edit (reduced y scale)

                new_cell_count++;

                new_cells.push_back({old->pos[0], edit->pos[1], old->pos[2],
                                     old->siz[0], edit->siz[1], oztoez - 1,
                                     old->mat});
            }

            if(efztoofz > 0) {
                // creating a cell in front of edit (reduced y scale)

                new_cell_count++;

                new_cells.push_back({old->pos[0], edit->pos[1], edit->pos[2] + edit->siz[2] + 1,
                                     old->siz[0], edit->siz[1], efztoofz - 1,
                                     old->mat});
            }

            if(oxtoex > 0) {
                // creating a cell to the left of edit

                new_cell_count++;

                new_cells.push_back({old->pos[0], edit->pos[1], edit->pos[2],
                                     oxtoex - 1, edit->siz[1], edit->siz[2],
                                     old->mat});
            }

            if(efxtoofx > 0) {
                // creating a cell to the right of edit

                new_cell_count++;

                new_cells.push_back({edit->pos[0] + edit->siz[0] + 1, edit->pos[1], edit->pos[2],
                                     efxtoofx - 1, edit->siz[1], edit->siz[2],
                                     old->mat});
            }


            // old takes up edits volume,
            // since the rest of the volume should be covered by the new cells
            *old = *edit;


        }

        return new_cell_count;
    }


    ///////////////////////////////////// functions to modify DrawChunks ///////////////////////////////////



    void EditChunk::setCell(const Cell& c, DrawChunk* chunk) {
        /** sets everything inside the cell to be the material of the cell
        * will make sure that the DrawChunk gets updated correctly
        * but maybe not in the most efficient way */

        loadDrawChunk(chunk);

        std::vector<Cell*> to_update; // the cells that need to be updated
        std::vector<Cell> updates; // the updates to those cells

        getCellsSharingVolume(c, to_update, updates);

        // updating the cells

        for(int i = 0; i < to_update.size(); i++) {

            std::vector<Cell> new_cells; // the new cells created in the process
            editCell(to_update[i], &updates[i], new_cells);

            // updating the references to the new cells
            for(Cell& nc : new_cells) {

                chunk->m_cells.push_back(nc);
                updateRefs(nc, chunk->m_cells.size() - 1);
            }

        }

    }


    Cell& EditChunk::getCell(unsigned char x, unsigned char y, unsigned char z, DrawChunk* chunk) {
        /** @return the cell at the position
        * you may even change its material (nothing else though)
        * dont expect any drawn changes before chunk->updateCellBuffer was called though */

        loadDrawChunk(chunk);

        return m_draw_chunk->m_cells[m_refs[x][y][z]];
    }

    unsigned short EditChunk::getMaterial(unsigned char x, unsigned char y, unsigned char z, DrawChunk* chunk) {

        return getCell(x,y,z, chunk).mat;
    }

    ////////////////////////////// functions to reduce the size of DrawChunks //////////////////////////////


    void EditChunk::optDrawChunkByMerge(DrawChunk* c) {
        /** tries to minimize the size of the DrawChunk
        * by finding cells that can be merged */

    }


    void EditChunk::optDrawChunkComplete(DrawChunk* c) {
        /** tries to minimize the size of the DrawChunk
        * probably slower but more effective then optDrawChunkByMerge */

    }

} // cell
