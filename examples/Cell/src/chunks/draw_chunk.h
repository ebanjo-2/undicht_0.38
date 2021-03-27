#ifndef DRAW_CHUNK_H
#define DRAW_CHUNK_H

#include <buffer_layout.h>
#include <array>
#include <vector>

#include <core/vertex_buffer.h>

#include <chunks/cell.h>

namespace cell {

    class EditChunk;
	class FastAccessChunk;
    class CellRenderer;

    class DrawChunk {
            /** a class that contains all the data needed to draw a chunk
            * with a side length of 256 units */

            friend EditChunk;
			friend FastAccessChunk;
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

            unsigned int m_drawn_cells = 0;

        public:

            /** stores the data from m_cells in m_buffer
            * so that the gpu can access it */
            void updateCellBuffer();

            unsigned int getDrawnCellCount();

        public:

            DrawChunk();
            virtual ~DrawChunk();

    };

} // cell

#endif // DRAW_CHUNK_H
