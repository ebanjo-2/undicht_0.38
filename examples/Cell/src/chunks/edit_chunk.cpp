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

        for(int x = c.getPoint1()[0]; x < c.getPoint2()[0] + 1; x++) {
            for(int y = c.getPoint1()[1]; y < c.getPoint2()[1] + 1; y++) {
                for(int z = c.getPoint1()[2]; z < c.getPoint2()[2] + 1; z++) {
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

        if(m_draw_chunk->m_cells.size() > c.getVolume()) {
            // there is more cells in the draw chunk then reference within c

            for(int x = c.getPoint1()[0]; x < c.getPoint2()[0] + 1; x++) {
                for(int y = c.getPoint1()[1]; y < c.getPoint2()[1] + 1; y++) {
                    for(int z = c.getPoint1()[2]; z < c.getPoint2()[2] + 1; z++) {
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
                            shared_volume.push_back(canditate->getSharedVolume(c));

                        }

                    }
                }
            }

        } else {
            // its faster to just test every cell in m_draw_chunk

            for(Cell& candidate : m_draw_chunk->m_cells) {

                if(candidate.sharesVolume(c)) {

                    loadTo.push_back(&candidate);
                    shared_volume.push_back(candidate.getSharedVolume(c));
                }

            }

        }

    }



    unsigned int EditChunk::editCell(Cell* old, Cell* edit, std::vector<Cell>& new_cells) {
        /** @brief calculates the changes needed to set "edit" into the old cell
        * if old and edit share the same material, nothing happens
        * if old and edit share the same volume, olds material will be set to edits
        * if edit is part of old,
        *   + new cells will be created to take up the volume difference between old and edit (they will have old material)
        *   + the number of new cells will be returned
        *   + old will be replaced with edit */

        size_t last_count = new_cells.size();

        old->setInCell(*edit, new_cells);

        return new_cells.size() - last_count;
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
