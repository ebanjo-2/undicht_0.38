#include "world/world_chunk.h"
#include <geometry/geometry.h>
#include <math/cell_math.h>


namespace cell {

    using namespace undicht;
    using namespace tools;

    WorldChunk::WorldChunk() {

        initDrawBuffer();
    }


    WorldChunk::WorldChunk(const glm::ivec3& origin) {

        initDrawBuffer();
        setOrigin(origin);
    }

    bool WorldChunk::isInside(const glm::ivec3& pos) {
        /** @return true, if the pos is within the 255*255*255 chunk starting at getOrigin() */

        glm::ivec3 d = pos - m_origin;

        return isPointInside(TCell<int>(glm::ivec3(0,0,0), glm::ivec3(255,255,255)), d);
    }

    Cell WorldChunk::getCellAt(const glm::ivec3& pos) {
        /** the pos actually has to be within the chunk,
        * otherwise its not defined which cell gets returned */

        u8vec3 cpos = u8vec3(pos - m_origin);

        int i = getCellIDAt(cpos);

        return m_cells[i];
    }

    /////////////////////////////// estimating the optimization need for the chunk ///////////////////////////////

    int WorldChunk::getOptNeed() const {
        /** @return a rough estimation of how much a optimization of the chunk is needed
        * 0 can mean that there is no need, the heigher the return,
        * the more cells were added to the chunk and the heigher the need for optimization */

        return getCellCount() - m_last_optimized_cell_count;
    }

    void WorldChunk::setOptNeed(int need) {
        /** may be used when a chunk is loaded from a file */

        m_last_optimized_cell_count = getCellCount() - need;
    }

    void WorldChunk::markAsOptimzed() {
        /** tells the chunk that its just been optimized
        * getOptNeed() will now return 0 until changes are made to the chunk*/

        m_last_optimized_cell_count = getCellCount();
    }


    const glm::ivec3& WorldChunk::getOrigin() const {

        return m_origin;
    }


    void WorldChunk::setOrigin(const glm::ivec3& origin) {

        m_origin = origin;
    }

    /////////////////////// adding / removing cells from m_cells  /////////////////////
    /////////////////////// + updating the draw buffer afterwards /////////////////////

    int WorldChunk::addCell(const Cell& c) {

        updateDrawBuffer(Chunk::addCell(c));
    }

    void WorldChunk::remCell(int id) {

        Chunk::remCell(id);
        updateDrawBuffer(id);
    }

    ///////////////////// creating and maintaining the data to draw the chunk ///////////////////

    void WorldChunk::initDrawBuffer() {

        m_vertex_buffer.setLayout(BufferLayout({UND_VEC3F, UND_VEC2F, UND_FLOAT}));
        m_vertex_buffer.setInstanceLayout(BufferLayout({UND_VEC3UI8, UND_VEC3UI8, UND_UINT16, UND_UINT8}));

        // creating the cube that gets drawn for every cell
        std::vector<float> vertices;

        Geometry::buildUVs(true);
        Geometry::buildNormals(false);
        Geometry::genCuboid(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(0.0f, 0.0f, 0.0f), vertices);

        // adding the face mask to the vertices
        int vertex_count = vertices.size() / 5;
        for(int i = 0; i < vertex_count; i++)
            vertices.insert(vertices.begin() + i * 6 + 5, (float)FACE_MASK[i / 6]);

        m_vertex_buffer.setData(vertices);
    }


    void WorldChunk::resizeDrawBuffer(int cell_count) {
        /** resizes the buffer that stores the data for every cell within the chunk
        * + fills the resized buffer with all the cells of the chunk */

        m_vertex_buffer.resizeInstanceBuffer(m_cells.size() * 9 * 2);

        for(int i = 0; i < m_cells.size(); i++ ) {

            updateDrawBuffer(i);
        }
    }


    void WorldChunk::updateDrawBuffer(int id) {
        /** updates the data of the cell within the vertex_buffer
        * will also resize the buffer to fit new cells */

        Cell& c = m_cells[id];

        unsigned char mat0 = c.m_material / 256;
        unsigned char mat1 = c.m_material % 256;


        unsigned char data[] = {

            c.m_pos0.x, c.m_pos0.y, c.m_pos0.z,
            c.m_pos1.x, c.m_pos1.y, c.m_pos1.z,
            mat1, mat0, // i was pretty sure it had to be mat0, mat1    but only this works (even with numbers > 255) edit: little endian
            c.m_visible_faces
        };

        if(id < m_vertex_buffer.getInstanceBufferSize() / 9) {

            m_vertex_buffer.setInstanceData(data, sizeof(char) * 9, id * sizeof(char) * 9);

        } else {
            // instance buffer needs to be resized
            // and every cell needs to be rewritten to that buffer
            resizeDrawBuffer((m_cells.size() + 1) * 9 * 2);
        }

    }

    void WorldChunk::updateDrawBuffer() {
        /** updates the draw buffer for every cell of the chunk */

        for(int i = 0; i < m_cells.size(); i++) {

            updateDrawBuffer(i);
        }

    }

    undicht::graphics::VertexBuffer& WorldChunk::getDrawBuffer(){

        return m_vertex_buffer;
    }




} // cell
