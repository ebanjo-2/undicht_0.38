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

            /*friend EditChunk;
			friend FastAccessChunk;
            friend CellRenderer;*/

        protected:
            // cube vertices

            static std::array<float, 12 * 3 * 8> s_cube_vertices;
            static undicht::tools::BufferLayout s_cube_layout;

            static bool s_cube_initialized;

            static void initCubeVertices();

        protected:
            // storing the data for the cells within the chunk

            std::vector<Cell> m_visible_cells; // on ram for the cpu
			std::vector<Cell> m_invisible_cells;
			std::vector<int> m_cells_to_update;
			std::vector<int> m_cells_unused; // cells that are no longer being drawn and therefor could be recycled

            undicht::graphics::VertexBuffer m_buffer; // on vram for the gpu (only visible)

			void writeToBuffer(const std::vector<Cell>& cells, int offset);
		
		public:

			/** adds the cell to the m_cells vector */
			virtual int addCell(const Cell& c);

			/** updates the existing cell 
			* @return: if the cell is turned visible or invisible a new id will be assigned */
			virtual int updateCell(const Cell& c, int id);

			virtual const Cell& getCell(int id);

        public:

            void updateCellBuffer();

			/** needed when the cell buffer is resized */
			void updateCellBufferTotal();

			/** updates the cells that are changed since the last time this was called */
			void updateCellBufferOnlyNew();

            unsigned int getDrawnCellCount();

			unsigned int getTotalCellCount();


        public:

            DrawChunk();
            virtual ~DrawChunk();

    };

} // cell

#endif // DRAW_CHUNK_H
