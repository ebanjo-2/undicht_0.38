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

    class CellRenderer;

    class Chunk {
            // stores cells in a 255 * 255 * 255 volume

        protected:

            friend CellRenderer;

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

        public:

            Chunk();
            virtual ~Chunk();

        public:
            // managing the edit cells
            // edit cells store a reference to the cells of the chunk
            // in an 256 * 256 * 256 array

            /** will need 256 * 256 * 256 * sizeof(int) bytes (67 MB for 32 bit integer)
            * on my old laptop this took ~80ms (without cells stored in the chunk),
            * so use this function wisely (time depends heavily on the number of cells in the chunk)*/
            void initEditCells();

            /** will free the memory used by the cell references */
            void termEditCells();

            void setCellRef(const Cell& c, int ref_id);

            int getCellRef(const u8vec3& pos) const;

            int getCellRef(unsigned char x, unsigned char y, unsigned char z) const;

        public:
            // editing cells (only call any of these

            /** @return 0 if the volume is empty */
            const Cell* getCell(const u8vec3& pos) const;

            /** finds all the cells in the volume of the cuboid described by pos0 and pos1 */
            void getCells(const u8vec3& ppos0, const u8vec3& ppos1, std::vector<int>& loadTo) const;

            void getCellsBySearchingAll(const u8vec3& ppos0, const u8vec3& ppos1, std::vector<int>& loadTo) const;

            void getCellsBySearchingVol(const u8vec3& ppos0, const u8vec3& ppos1, std::vector<int>& loadTo) const;

            /** sets multiple cells */
            void setCells(const std::vector<Cell>& cells);

            /** unless the edit cells are initialized,
            * this does not check if the space was already used by another cell
            * so be careful with setting cells without the edit cells */
            void setCell(const Cell& c);

            /** adding the cell to the chunks array of cells
            * without checking for existing cells in the new cells space */
            void setCellBlind(const Cell& c, int id = -1);


        public:
            // creating and maintaining the data to draw the chunk

            void resizeDrawBuffer(int cell_count);

            /** assuming the cell was already stored in the draw buffer */
            void updateDrawBuffer(int id);

            int getCellCount() const;

        public:
            // deciding which faces of cells cant be seen and therefor shouldnt be rendered

            unsigned char calcVisibleFaces(const Cell& c);

            //void updateVisibleFaces();

            /** @return true, if the cell ref at pos is not -1
            * note that this function takes an ivec3, which means the values can exceed the range of 0 to 255
            * (i.e. when checking on a chunks edge)
            * in which case this function will always return false
            * it may be a good idea to actually take other chunks into account in the future */
            bool isCellSolid(const glm::ivec3& pos);



    };


} // cell


#endif // CHUNK_H
