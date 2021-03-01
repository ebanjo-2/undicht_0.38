#include "edit_chunk.h"
#include <iostream>

namespace cell {

    EditChunk::EditChunk() {

        // resizing the m_cells buffers
        // (wild speculation:) i think it is faster to allocate for a lot of smaller buffers
        // instead of one big buffer
        m_cells.resize(256);

        for(auto& v : m_cells) {

            v.resize(256);

            for(auto& vv : v) {

                vv.resize(256);
            }

        }

    }

    EditChunk::~EditChunk() {


    }


    ///////////////////////////////////// functions to create the DrawChunk /////////////////////////////////////

    void EditChunk::clearMAddStencil() {
        /** clears all values in m_add_stencil to false */

        // resizing will also set the values to false
        // new memory is only allocated if the vector former size was smaller then the new one
        m_add_stencil.resize(256);

        for(auto& v : m_add_stencil) {
            v.resize(256);
            for(auto& vv : v) {
                vv.resize(256);
            }

        }
    }


    void EditChunk::setInMAddStencil(const Cell& c, bool b) {

        unsigned int max_x = c.pos[0] + c.siz[0] + 1;
        unsigned int max_y = c.pos[1] + c.siz[1] + 1;
        unsigned int max_z = c.pos[2] + c.siz[2] + 1;

        if(max_x > 256) return;
        if(max_y > 256) return;
        if(max_z > 256) return;


        for(unsigned int x = c.pos[0]; x < max_x; x++) {
            for(unsigned int y = c.pos[1]; y < max_y; y++) {
                for(unsigned int z = c.pos[2]; z < max_z; z++) {

                    m_add_stencil[x][y][z] = b;

                }
            }
        }

    }


    bool EditChunk::sameMaterial(const Cell& c) {
        /** checks if the material is the same everywhere within the cell
        * + if they are all marked with false in the m_add_stencil buffer */

        unsigned int max_x = c.pos[0] + c.siz[0] + 1;
        unsigned int max_y = c.pos[1] + c.siz[1] + 1;
        unsigned int max_z = c.pos[2] + c.siz[2] + 1;

        if(max_x > 256) return false;
        if(max_y > 256) return false;
        if(max_z > 256) return false;

        for(unsigned int x = c.pos[0]; x < max_x; x++) {
            for(unsigned int y = c.pos[1]; y < max_y; y++) {
                for(unsigned int z = c.pos[2]; z < max_z; z++) {

                        if(!m_add_stencil[x][y][z]) {
                            // testing if the cell was added to the add stencil buffer
                            if(m_cells[x][y][z] != c.mat) {
                                // there is a cell within the volume with a wrong material
                                return false;
                            }

                        } else {

                            return false;
                        }

                }
            }
        }

        return true;
    }



    ///////////////////////////////////// converting from DrawChunk to EditChunk and back /////////////////////////////////////

    void EditChunk::loadCells(const DrawChunk& c) {
        /** loads the DrawChunk Cells to the EditChunks cell buffer */

        for(const Cell& cell : c.m_cells) {

            for(int x = cell.pos[0]; x < cell.pos[0] + cell.siz[0]; x++) {
                for(int y = cell.pos[1]; y < cell.pos[1] + cell.siz[1]; y++) {
                    for(int z = cell.pos[2]; z < cell.pos[2] + cell.siz[2]; z++) {

                        m_cells[x][y][z] = cell.mat;

                    }
                }
            }

        }

    }

    void EditChunk::updateDrawChunk(DrawChunk& c) {
        /** updates the DrawChunk so that the edited cells can get displayed */

        // position of the next cell
        unsigned short x = 0;
        unsigned short y = 0;
        unsigned short z = 0;

        // size of the next cell
        unsigned short sx = 0;
        unsigned short sy = 0;
        unsigned short sz = 0;

        // in what directions to search for cells with the same material
        bool searchx = true;
        bool searchy = true;
        bool searchz = true;

        // a buffer that stores whether or not a cell was added to a bigger cell
        clearMAddStencil();

        // finding cells with the same materials
        bool next_cell_found = true;
        while(next_cell_found) {

            // finding the next cell that wasnt added to a bigger cell jet
            next_cell_found = false;

            for(x = 0; x < 256; x++) {
                for(y = 0; y < 256; y++) {
                    for(z = 0; z < 256; z++) {
                        if(!m_add_stencil[x][y][z]) {
                            next_cell_found = true;
                            break;
                        }
                    }

                    if(next_cell_found) break;
                }

                if(next_cell_found) break;
            }

            if(!next_cell_found) break;

            // searching for cells with the same material that would fit into the bigger cell
            searchx = true;
            searchy = true;
            searchz = true;

            sx = 0;
            sy = 0;
            sz = 0;

            unsigned short current_mat = m_cells[x][y][z];

            setInMAddStencil({x,y,z,0,0,0,0}, true);

            while(searchx || searchy || searchz) {

                if(searchx) searchx = sameMaterial({x + sx + 1,y,z, 0,sy,sz, current_mat});
                if(searchx) sx += 1;

                if(searchy) searchy = sameMaterial({x,y + sy + 1,z, sx,0,sz, current_mat});
                if(searchy) sy += 1;

                if(searchz) searchz = sameMaterial({x,y,z + sz + 1, sx,sy,0, current_mat});
                if(searchz) sz += 1;

                if(searchx) setInMAddStencil({x + sx,y,z, 0,sy,sz, 0}, true);
                if(searchy) setInMAddStencil({x,y + sy,z, sx,0,sz, 0}, true);
                if(searchz) setInMAddStencil({x,y,z + sz, sx,sy,0, 0}, true);

            }



            std::cout << "constructed cell: " << x << " " << y << " " << z << " : " << sx << " " << sy << " " << sz << " mat: " << current_mat << "\n";

            // adding the new cell to the draw chunk
            c.m_cells.push_back({x,y,z,sx,sy,sz,current_mat});

        }

        c.updateCellBuffer();
        std::cout << "DrawChunk has " << c.m_cells.size() << " cells " << "\n";
    }


    /////////////////////////////////////////// setting / getting cell data ///////////////////////////////////////////

    void EditChunk::setCell(const Cell& c) {

        for(int x = c.pos[0]; x < c.pos[0] + c.siz[0] + 1 ; x++) {
            for(int y = c.pos[1]; y < c.pos[1] + c.siz[1] + 1 ; y++) {
                for(int z = c.pos[2]; z < c.pos[2] + c.siz[2] + 1 ; z++) {

                    m_cells[x][y][z] = c.mat;
                }
            }
        }

    }

    unsigned short EditChunk::getCell(unsigned char x, unsigned char y,unsigned char z) {

        return m_cells[x][y][z];
    }

} // cell
