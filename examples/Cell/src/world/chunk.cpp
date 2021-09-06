#include <world/chunk.h>

#include <geometry/geometry.h>

namespace cell {

    using namespace undicht;
    using namespace tools;

    const unsigned char YP = 0x01; // + y (00000001)
    const unsigned char YN = 0x02; // - y (00000010)
    const unsigned char XP = 0x04; // + x (00000100)
    const unsigned char XN = 0x08; // - x (00001000)
    const unsigned char ZP = 0x10; // + z (00010000)
    const unsigned char ZN = 0x20; // - z (00100000)

    const unsigned char FACE_MASK[] = {YP, YN, XP, XN, ZP, ZN};

    int Chunk::getCellCount() const {


        return m_cells.size();
    }


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
        // using fill_n to set the reference in the x direction of the cell

    }

    ///////////////////// creating and maintaining the data to draw the chunk ///////////////////

    void Chunk::initDrawBuffer() {
        /** the buffer which is used to draw the cells */

        // creating the cube that gets drawn for every cell
        std::vector<float> vertices;

        Geometry::buildUVs(true);
        Geometry::buildNormals(false);
        Geometry::genCuboid(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(0.0f, 0.0f, 0.0f), vertices);

        // adding the face mask to the vertices
        int vertex_count = vertices.size() / 5; // currently 5 floats per vertex


        for(int i = 0; i < vertex_count; i++) {

            unsigned char face = FACE_MASK[i / 6];
            vertices.insert(vertices.begin() + i * 6 + 5, (float)face);
        }

        m_vertex_buffer.setLayout(BufferLayout({UND_VEC3F, UND_VEC2F, UND_FLOAT}));
        m_vertex_buffer.setData(vertices);

        // 3 bytes pos0, 3 bytes pos1, 2 bytes material, 1 byte mask of faces to draw
        m_vertex_buffer.resizeInstanceBuffer(m_cells.size() * 9);
        m_vertex_buffer.setInstanceLayout(BufferLayout({UND_VEC3UI8, UND_VEC3UI8, UND_UINT16, UND_UINT8}));

        for(int i = 0; i < m_cells.size(); i++ ) {

            updateDrawBuffer(m_cells[i], i);
        }
    }


    void Chunk::updateDrawBuffer(const Cell& c, int id) {

        unsigned char mat0 = c.m_material / 256;
        unsigned char mat1 = c.m_material % 256;

        unsigned char visible_faces = 0xFF; // 11111111, all faces visible;

        if(m_visible_faces.size() > id) {

            visible_faces = m_visible_faces[id];
        }

        unsigned char data[] = {

            c.m_pos0.x, c.m_pos0.y, c.m_pos0.z,
            c.m_pos1.x, c.m_pos1.y, c.m_pos1.z,
            mat1, mat0, // i was pretty sure it had to be mat0, mat1    but only this works (even with numbers > 255)
            visible_faces
        };

        m_vertex_buffer.setInstanceData(data, sizeof(char) * 9, id * sizeof(char) * 9);

    }

    ///////////////////////////// deciding which faces of cells cant be seen and therefor shouldnt be rendered /////////////////////////////

    unsigned char Chunk::calcVisibleFaces(const Cell& c) {

        if(!layer.size()) return 255; // edit cells not initialized

        u8vec3 pos0 = glm::min(c.m_pos0, c.m_pos1);
        u8vec3 pos1 = glm::max(c.m_pos0, c.m_pos1);

        // vertices of all the faces along which the neighbouring cells need to be checked
        std::array<u8vec3, 12> faces = {
            u8vec3(pos0.x, pos1.y + 1, pos0.z), u8vec3(pos1.x, pos1.y + 2, pos1.z), // +y
            u8vec3(pos0.x, pos0.y - 1, pos0.z), u8vec3(pos1.x, pos0.y - 0, pos1.z), // -y
            u8vec3(pos1.x + 1, pos0.y, pos0.z), u8vec3(pos1.x + 2, pos1.y, pos1.z), // +x
            u8vec3(pos0.x - 1, pos0.y, pos0.z), u8vec3(pos0.x - 0, pos1.y, pos1.z), // -x
            u8vec3(pos0.x, pos0.y, pos1.z + 1), u8vec3(pos1.x, pos1.y, pos1.z + 2), // +z
            u8vec3(pos0.x, pos0.y, pos0.z - 1), u8vec3(pos1.x, pos1.y, pos0.z - 0), // -z
        };

        unsigned char visible_faces = 0x00;

        for(int i = 0; i < 6; i++) {

            for(int x = faces[i * 2 + 0].x; x < faces[i * 2 + 1].x; x++) {

                for(int y = faces[i * 2 + 0].y; y < faces[i * 2 + 1].y; y++) {

                    for(int z = faces[i * 2 + 0].z; z < faces[i * 2 + 1].z; z++) {

                        if(layer[y].row[z].cell[x] == -1) {
                            // there is no solid cell at that position
                            visible_faces = visible_faces | FACE_MASK[i];
                            break;
                        }

                    }

                    if(FACE_MASK[i] & visible_faces) break;
                }

                if(FACE_MASK[i] & visible_faces) break;
            }

        }

        return visible_faces;
    }

    void Chunk::updateVisibleFaces() {

        m_visible_faces.resize(m_cells.size());

        for(int i = 0; i < m_cells.size(); i++) {

            Cell& c = m_cells[i];
            m_visible_faces[i] = calcVisibleFaces(c);

        }

    }






} // cell
