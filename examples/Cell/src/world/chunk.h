#ifndef CHUNK_H
#define CHUNK_H


#include <world/cell.h>
#include <array>
#include <vector>

#include <core/vertex_buffer.h>



namespace cell {

    struct ChunkRow {
        /** a row (x direction) of 256 references to cells */

        std::array<int, 256> cell;

        ChunkRow() {
            // init all cell references to -1 (not referencing a cell, indicating the volume is empty)
            cell.fill(-1);
        }
    };

    struct ChunkLayer {
        /** contains 256 rows of cells, forming a layer of 256 * 256 cell references */

        std::array<ChunkRow, 256> row;
    };

    class Chunk {
            // stores cells in a 255 * 255 * 255 volume

        public:

            /** used for quickly addressing a single cell, i.e. for editing
            * since 256 layers would take up a lot of memory, distant chunks
            * should delete them */
            std::vector<ChunkLayer> layer;

            /** since i expect cells with the same material to be clustered,
            * this should be a memory efficient way of storing them
            * spaces without cells should be considered empty (air/void) */
            std::vector<Cell> m_cells;

            std::vector<unsigned char> m_visible_faces;

            /** contains data about every cell in the chunk
            * 6 byte positions ( 2 * 3 bytes for u8vec)
            * 2 byte material id
            * 1 byte face mask (stores what face of the cell should get drawn */
            undicht::graphics::VertexBuffer m_vertex_buffer;

            int getCellCount() const;

        public:
            // editing cells within the chunk

            /** will need 256 * 256 * 256 * sizeof(int) bytes (67 MB for 32 bit integer)
            * on my old laptop this took ~80ms (without cells stored in the chunk),
            * so use this function wisely (time depends heavily on the number of cells in the chunk)*/
            void initEditCells();

            /** will free the memory used by the cell references */
            void termEditCells();

            void setCellRef(const Cell& c, int ref_id);

        public:
            // creating and maintaining the data to draw the chunk

            /** the buffer which is used to draw the cells */
            void initDrawBuffer();

            void updateDrawBuffer(const Cell& c, int id);


        public:
            // deciding which faces of cells cant be seen and therefor shouldnt be rendered

            unsigned char calcVisibleFaces(const Cell& c);

            void updateVisibleFaces();



    };


} // cell


#endif // CHUNK_H
