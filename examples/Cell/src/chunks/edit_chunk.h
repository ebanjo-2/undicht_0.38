#ifndef EDIT_CHUNK_H
#define EDIT_CHUNK_H

#include "draw_chunk.h"
#include <vector>

namespace cell {

    class EditChunk {
            /** contains the data of a chunk that can be modified
            * it will (in most cases) take up significantly more memory than a draw chunk, so use it wisely */

        public:
            // contains the material value for each of the 256 * 256 * 256 cells within the chunk
            // thats 2 bytes * 256 * 256 * 256 (about 34 megabytes)
            // so it has to be allocated on heap memory
            std::vector<std::vector<std::vector<unsigned short>>> m_cells;

            // used to convert the EditChunks Cells to DrawChunks Cells
            std::vector<std::vector<std::vector<bool>>> m_add_stencil;


        public:
            // functions to create the DrawChunk

            /** clears all values in m_add_stencil to false */
            void clearMAddStencil();

            void setInMAddStencil(const Cell& c, bool b);

            /** checks if the material is the same everywhere within the cell
            * + if they are all marked with false in the m_add_stencil buffer */
            bool sameMaterial(const Cell& c);



        public:
            // converting from DrawChunk to EditChunk and back

            /** loads the DrawChunk Cells to the EditChunks cell buffer */
            void loadCells(const DrawChunk& c);

            /** updates the DrawChunk so that the edited cells can get displayed */
            void updateDrawChunk(DrawChunk& c);

        public:
            // setting / getting cell data

            void setCell(const Cell& c);

            unsigned short getCell(unsigned char x, unsigned char y,unsigned char z);

        public:

            EditChunk();
            virtual ~EditChunk();

    };

} // cell


#endif // EDIT_CHUNK_H
