// created in the summer of 2021
// improving on the previous attempt at creating a fast to edit voxel world
// this version does not use ids stored for every position within the chunk to edit
// improving the memory uasage

// how the editing process works:
// - through the function setCell() a new cell is requested to be placed within the chunk
// - and to replace the cells that were present within that volume before
//
// - first, every cell that is of interest for the operation is determined
// - this does not only include the cells that are going to be replaced,
// - but also the ones next to the new cell,
// - since they determine which faces of the new cell can be visible
//
// - based on those cells, it is calculated which faces of the new cell can be visible
// - and the new cell is added to the vector of cells within the chunk
//
// - removing the volume of the new cell from the ones that existed in the volume before
// - is done via subtractFromCell()
// - setting a cell within the volume of another cell means that the old cell
// - is split up into a maximum of 6 new cells
//
// - lastly the changes are made visible by updating the draw buffer

// what are mini chunks?
// - the volume of a Chunk is 255 * 255 * 255
// - they can contain a lot of cells ( > 100.000)
// - searching for cells that fill a volume / contain a position would mean going through all of them
// - (worst case), which would slow down editing
// - mini chunks cover a volume of 15 * 15 * 15 (16 * 16 * 16 mini chunks per chunk)
// - they store the cells present within their volume
// - so instead of going through every cell within the chunk to find the one containing a position
// - it is only neseccary to go through every cell within the mini chunk of that position
// - to get the mini chunks that cover the volume of a cell use calcMiniChunks()


#ifndef CHUNK_H
#define CHUNK_H


#include <world/cell.h>
#include <array>
#include <vector>

#include <core/vertex_buffer.h>



namespace cell {

    class CellRenderer;
    class ChunkOptimizer;
    class WorldFile;

    class Chunk {
            // stores cells in a 255 * 255 * 255 volume

        public:

            friend CellRenderer;
            friend ChunkOptimizer;
            friend WorldFile;

            /** since i expect cells with the same material to be clustered,
            * this should be a memory efficient way of storing them
            * Cells that contain void (and shouldnt be rendered)
            * contain the material id VOID_CELL */
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

            /** recycles previously deleted Cell memory (if possible)
            * + adds the cell to the mini chunk system
            * so dont just use m_cells.push_back to add cells */
            int addCell(const Cell& c);

            void remCell(int id);


        protected:
            // managing the mini chunks

            // "mini chunks" are lists of cells that are present within 16 * 16 * 16 cubes within the big chunk
            // they should hopefully speed up the search for cells
            // without using to much memory

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

            virtual void setCell(const Cell& c);

            /** c has to be a part of the cell with no volume outside */
            void subtractFromCell(int cell_id, const Cell& c);

            /** @return true, if the sum of all cells fills the entire chunk
            * (debug function to test editing) */
            bool validVolume();

        public:
            // creating and maintaining the data to draw the chunk

            /** resizes the buffer that stores the data for every cell within the chunk
            * + fills the resized buffer with all the cells of the chunk */
            void resizeDrawBuffer(int cell_count);

            /** updates the data of the cell within the vertex_buffer
            * will also resize the buffer to fit new cells */
            void updateDrawBuffer(int id);

            /** updates the draw buffer for every cell of the chunk */
            void updateDrawBuffer();

            int getCellCount() const;

        public:
            // deciding which faces of cells cant be seen and therefor shouldnt be rendered

            /** checks for every face if there are VOID_CELLS in front of it
            * if so, the face will be marked visible
            * the face will also be visible if its at the chunk edge
            * cells with material type VOID_CELL always have no visible faces */
            unsigned char calcVisibleFaces(const Cell& c, const std::vector<int>& cell_pool);

            /** @return true, if there are empty cells within the volume
            * or if the volume goes outside the chunk range of [0,255]*/
            bool hasVoidCells(const glm::ivec3& ppos0, const glm::ivec3& ppos1, const std::vector<int>& cell_pool);

    };


} // cell


#endif // CHUNK_H
