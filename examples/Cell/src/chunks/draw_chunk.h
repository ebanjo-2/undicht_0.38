#ifndef DRAW_CHUNK_H
#define DRAW_CHUNK_H

#include <buffer_layout.h>
#include <array>
#include <vector>

#include <core/vertex_buffer.h>

namespace cell {

    struct Cell {

        unsigned short pos[3];
        unsigned short siz[3];
        unsigned short mat;

    };

    class EditChunk;
    class CellRenderer;

    class DrawChunk {
            /** a class that contains all the data needed to draw a chunk
            * with a side length of 256 units */

            friend EditChunk;
            friend CellRenderer;

        protected:
            // cube vertices

            static std::array<float, 12 * 3 * 8> s_cube_vertices;
            static undicht::tools::BufferLayout s_cube_layout;

            static bool s_cube_initialized;

            static void initCubeVertices();

        protected:
            // storing the data for the cells within the chunk

            std::vector<Cell> m_cells; // on ram for the cpu
            undicht::graphics::VertexBuffer m_buffer; // on vram for the gpu

            /** stores the data from m_cells in m_buffer
            * so that the gpu can access it */
            void updateCellBuffer();

        public:

            DrawChunk();
            virtual ~DrawChunk();

    };

} // cell

#endif // DRAW_CHUNK_H
