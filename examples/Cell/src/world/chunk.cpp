#include <world/chunk.h>

#include <geometry/geometry.h>

namespace cell {

    using namespace undicht;
    using namespace tools;

    //////////////////////////////////////// editing cells within the chunk //////////////////////////////////////


    void Chunk::initEditCells() {
        /** will need 256 * 256 * 256 * sizeof(int) bytes (67 MB for 32 bit integer) */

        layer.resize(256);

        for(int i = 0; i < m_cells.size(); i++) {

            Cell& c = m_cells[i];

            setCellRef(c, i);
        }

    }

    void Chunk::termEditCells() {
        /** will free the memory used by the cell references */

        // clear() is not guaranteed to free the memory
        // but this does the trick
        std::vector<ChunkLayer>().swap(layer);
    }

    void Chunk::setCellRef(const Cell& c, int ref_id) {

        std::cout << "setting ref " << ref_id << "\n";

        int x0 = std::min(c.m_pos0.x, c.m_pos1.x);
        int x1 = std::max(c.m_pos0.x, c.m_pos1.x);
        int y0 = std::min(c.m_pos0.y, c.m_pos1.y);
        int y1 = std::max(c.m_pos0.y, c.m_pos1.y);
        int z0 = std::min(c.m_pos0.z, c.m_pos1.z);
        int z1 = std::max(c.m_pos0.z, c.m_pos1.z);

        for(int y = y0; y < y1; y++)
            for(int z = z0; z < z1; z++)
                std::fill_n(layer[y].row[z].cell.begin() + x0, x1 - x0, ref_id);

    }
    ///////////////////// creating and maintaining the data to draw the chunk ///////////////////

    void Chunk::initDrawBuffer() {
        /** the buffer which is used to draw the cells */

        // creating the cube that gets drawn for every cell
        std::vector<float> vertices;
        std::vector<float> quad_vertices; // temporary vertex buffer to store the data of each face of the cube

        Geometry::useIndices(false);
        Geometry::buildUVs(true);
        Geometry::buildNormals(false);



    }

    void Chunk::updateDrawBuffer(const Cell& c, int id) {

    }


} // cell
