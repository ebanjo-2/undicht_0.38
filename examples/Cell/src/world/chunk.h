#ifndef CHUNK_H
#define CHUNK_H


#include <world/cell.h>
#include <array>
#include <vector>

#include <core/vertex_buffer.h>



namespace cell {

    class CellRenderer;

    class Chunk {
            // stores cells in a 255 * 255 * 255 volume

        public:

            friend CellRenderer;

            /** since i expect cells with the same material to be clustered,
            * this should be a memory efficient way of storing them
            * spaces without cells should be considered empty (air/void) */
            std::vector<Cell> m_cells;

            std::vector<int> m_unused_cells;

            std::array<std::array<std::array<std::vector<int>, 16>, 16>,16> m_mini_chunks;

            /** contains data about every cell in the chunk
            * 6 byte positions ( 2 * 3 bytes for u8vec)
            * 2 byte material id
            * 1 byte face mask (stores what face of the cell should get drawn */
            undicht::graphics::VertexBuffer m_vertex_buffer;

        public:

            Chunk();
            virtual ~Chunk();

        protected:
            // adding / removing cells from m_cells

            int addCell(const Cell& c);

            void remCell(int id);


        public:
            // managing the mini chunks

            // "mini chunks" are lists of cells that are present within 16 * 16 * 16 cubes within the big chunk
            // they should hopefully speed up the search for cells
            // without using to much memory

            // these functions are only public so chunk_optimization.h can acces them
            // otherwise they are purely internal

            // coords ranging from 0 to 15
            std::vector<int>& getMiniChunk(int x, int y, int z);

            // coords ranging from 0 to 15
            std::vector<int>& getMiniChunk(const u8vec3& pos);

            /** calculates which mini chunks c is part of */
            void calcMiniChunks(int cell_id, u8vec3& chunk0, u8vec3& chunk1);
            void calcMiniChunks(const Cell& c, u8vec3& chunk0, u8vec3& chunk1);

            void addToMiniChunks(int cell_id);

            void remFromMiniChunks(int cell_id);

        public:
            // searching cells

            int getCellAt(const u8vec3& pos);

            /** searches all of m_cells for cells that overlap with the volume */
            void getCellsInVolume(const Cell& volume, std::vector<int>& loadTo);

            /** searches the cells referenced in cell_pool for cells that overlap with the volume */
            void getCellsInVolume(const Cell& volume, std::vector<int>& loadTo, const std::vector<int>& cell_pool);

        public:
            // editing cells

            void setCells(const std::vector<Cell>& cells);

            void setCell(const Cell& c);

            /** c has to be a part of the cell with no volume outside */
            void subtractFromCell(int cell_id, const Cell& c);

            /** @return true, if the sum of all cells fills the entire chunk */
            bool validVolume();

        public:
            // creating and maintaining the data to draw the chunk

            void resizeDrawBuffer(int cell_count);

            void updateDrawBuffer(int id);

            int getCellCount() const;

        public:
            // deciding which faces of cells cant be seen and therefor shouldnt be rendered

            unsigned char calcVisibleFaces(const Cell& c, const std::vector<int>& cell_pool);

            /** @return true, if there are empty cells within the volume */
            bool hasVoidCells(const glm::ivec3& ppos0, const glm::ivec3& ppos1, const std::vector<int>& cell_pool);

    };


} // cell


#endif // CHUNK_H
