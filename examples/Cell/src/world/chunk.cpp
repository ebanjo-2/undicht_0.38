#include <world/chunk.h>
#include <algorithm>
#include <geometry/geometry.h>

#include <math/cell_math.h>

namespace cell {

    using namespace undicht;
    using namespace tools;



    Chunk::Chunk() {

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

        setCell(Cell(u8vec3(0,0,0), u8vec3(255, 255, 255), VOID_CELL));
    }


    Chunk::~Chunk() {

    }

    /////////////////////////// adding / removing cells from m_cells /////////////////////////////

    int Chunk::addCell(const Cell& c) {

        int id;

        if(m_unused_cells.size()) {

            id = m_unused_cells.back();
            m_unused_cells.pop_back();

        } else {

            id = m_cells.size();
            m_cells.resize(id + 1 );
        }

        m_cells[id] = c;

        return id;
    }

    void Chunk::remCell(int id) {

        m_unused_cells.push_back(id);
        m_cells[id] = Cell();
    }


    //////////////////////////////////// searching cells ////////////////////////////////////////


    void Chunk::getCellsInVolume(const Cell& volume, std::vector<int>& loadTo) {
        /** searches all of m_cells for cells that overlap with the volume */

        for(int i = 0; i < m_cells.size(); i++) {

            if(overlappingVolume(volume, m_cells[i])) {

                if(std::find(loadTo.begin(), loadTo.end(), i) == loadTo.end()) {

                    loadTo.push_back(i);
                }

            }

        }

    }

    void Chunk::getCellsInVolume(const Cell& volume, std::vector<int>& loadTo, const std::vector<int>& cell_pool) {
        /** searches the cells referenced in cell_pool for cells that overlap with the volume */

        for(int i : cell_pool) {

            if(overlappingVolume(volume, m_cells[i])) {

                if(std::find(loadTo.begin(), loadTo.end(), i) == loadTo.end()) {

                    loadTo.push_back(i);
                }

            }

        }

    }


    //////////////////////////////////// editing cells //////////////////////////////////////////


    /** sets multiple cells */
    void Chunk::setCells(const std::vector<Cell>& cells) {

        for(const Cell& c : cells) {

            setCell(c);
        }
    }

    void Chunk::setCell(const Cell& c) {

        glm::ivec3 ipos0 = glm::ivec3(glm::min(c.m_pos0, c.m_pos1)) - glm::ivec3(1,1,1);
        glm::ivec3 ipos1 = glm::ivec3(glm::max(c.m_pos0, c.m_pos1)) + glm::ivec3(1,1,1);

        u8vec3 pos0 = (u8vec3)glm::max(ipos0, glm::ivec3(0,0,0));
        u8vec3 pos1 = (u8vec3)glm::min(ipos1, glm::ivec3(255,255,255));

        Cell total_volume(pos0, pos1);

        std::vector<int> total_cell_pool; // every cell that might be of interest
        getCellsInVolume(total_volume, total_cell_pool);

        int c_id = addCell(c);

        m_cells[c_id].m_visible_faces = calcVisibleFaces(c, total_cell_pool);

        // modifying the cells that occupied the space before c
        for(int i : total_cell_pool) {

            subtractFromCell(i, getSharedVolume(c, m_cells[i]));
        }

        updateDrawBuffer(c_id);

    }



    // when creating the cells that fill the volume difference
    // between the original cell and c, some faces of these new cells may or may not be visible

    // copy the face setting from the original cell
    const std::array<unsigned char, 6> take_from_original = {
        ~YN & 0x3F,     // the cell in yp direction
        ~YP & 0x3F,     // the cell in yn direction
        XP | ZP | ZN,   // the cell in xp direction
        XN | ZP | ZN,   // the cell in xn direction
        ZP,             // the cell in zp direction
        ZN              // the cell in zn direction
    };

    // faces that might be inside the original cell
    const std::array<unsigned char, 6> self_covered {
        0x00,
        0x00,
        YP | YN,
        YP | YN,
        YP | YN | XP | XN,
        YP | YN | XP | XN
    };

    void Chunk::subtractFromCell(int cell_id, const Cell& c) {
        /** c has to be a part of the cell with no volume outside*/

        Cell original = m_cells[cell_id];

        // creating new cells taking up the volume difference between the original and c
        std::array<Cell, 6> new_cells;
        subtractCells(original, c, new_cells);

        // calculating which new cells exist
        unsigned char new_cell_mask = 0x00;

        for(int i = 0; i < 6; i++) {

            if(getVolume(new_cells[i])) {
                new_cell_mask |= (0x01 << i);
                new_cells[i].m_material = original.m_material;
            }

        }

        // calculating the visible faces of the new cells
        for(int i = 0; i < 6; i++) {

            if(original.m_material == VOID_CELL) continue; // no visible faces

            // cell faces taken from the original cell
            new_cells[i].m_visible_faces |= (original.m_visible_faces & take_from_original[i]);

            // cell faces that may be covered by other new cells
            new_cells[i].m_visible_faces |= ((~new_cell_mask) & self_covered[i] & original.m_visible_faces);

            // cell faces facing towards c
            if(c.m_material != VOID_CELL)
                new_cells[i].m_visible_faces |= OPPOSING_FACE_MASK[i];

        }

        // editing the m_cells array
        remCell(cell_id);
        updateDrawBuffer(cell_id);


        for(int i = 0; i < 6; i++) {

            if(new_cell_mask & FACE_MASK[i]) {

                int new_id = addCell(new_cells[i]);
                updateDrawBuffer(new_id);
            }

        }

    }

    bool Chunk::validVolume() {
        /** @return true, if the sum of all cells fills the entire chunk */

        int cell_vol = 0;

        for(Cell& c : m_cells) {

            cell_vol += getVolume(c);
        }

        return cell_vol == (255 * 255 * 255);
    }


    ///////////////////// creating and maintaining the data to draw the chunk ///////////////////


    void Chunk::resizeDrawBuffer(int cell_count) {

        m_vertex_buffer.resizeInstanceBuffer(m_cells.size() * 9 * 2);

        for(int i = 0; i < m_cells.size(); i++ ) {

            updateDrawBuffer(i);
        }
    }


    void Chunk::updateDrawBuffer(int id) {

        Cell& c = m_cells[id];

        unsigned char mat0 = c.m_material / 256;
        unsigned char mat1 = c.m_material % 256;


        unsigned char data[] = {

            c.m_pos0.x, c.m_pos0.y, c.m_pos0.z,
            c.m_pos1.x, c.m_pos1.y, c.m_pos1.z,
            mat1, mat0, // i was pretty sure it had to be mat0, mat1    but only this works (even with numbers > 255)
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


    int Chunk::getCellCount() const {

        return m_cells.size();
    }

    ///////////////////////////// deciding which faces of cells cant be seen and therefor shouldnt be rendered /////////////////////////////

    unsigned char Chunk::calcVisibleFaces(const Cell& c, const std::vector<int>& cell_pool) {

        if(c.m_material == VOID_CELL) return 0x00; // void cell

        glm::ivec3 pos0 = (glm::ivec3)glm::min(c.m_pos0, c.m_pos1);
        glm::ivec3 pos1 = (glm::ivec3)glm::max(c.m_pos0, c.m_pos1);

        // vertices of all the faces along which the neighbouring cells need to be checked
        std::array<glm::ivec3, 12> faces = {
            glm::ivec3(pos0.x, pos1.y + 0, pos0.z), glm::ivec3(pos1.x, pos1.y + 1, pos1.z), // +y
            glm::ivec3(pos0.x, pos0.y - 1, pos0.z), glm::ivec3(pos1.x, pos0.y - 0, pos1.z), // -y
            glm::ivec3(pos1.x + 0, pos0.y, pos0.z), glm::ivec3(pos1.x + 1, pos1.y, pos1.z), // +x
            glm::ivec3(pos0.x - 1, pos0.y, pos0.z), glm::ivec3(pos0.x - 0, pos1.y, pos1.z), // -x
            glm::ivec3(pos0.x, pos0.y, pos1.z + 0), glm::ivec3(pos1.x, pos1.y, pos1.z + 1), // +z
            glm::ivec3(pos0.x, pos0.y, pos0.z - 1), glm::ivec3(pos1.x, pos1.y, pos0.z - 0), // -z
        };

        unsigned char visible_faces = 0x00;

        for(int i = 0; i < 6; i++) {

            if(hasVoidCells(faces[i * 2 + 0], faces[i * 2 + 1], cell_pool))
                visible_faces |= FACE_MASK[i];

        }

        return visible_faces;
    }

    bool Chunk::hasVoidCells(const glm::ivec3& ppos0, const glm::ivec3& ppos1,  const std::vector<int>& cell_pool) {

        if((ppos0.x < 0) || (ppos1.x > 255)) return true;
        if((ppos0.y < 0) || (ppos1.y > 255)) return true;
        if((ppos0.z < 0) || (ppos1.z > 255)) return true;

        for(const int& i : cell_pool) {

            if(m_cells[i].m_material == VOID_CELL) {

                if(overlappingVolume(m_cells[i], Cell(u8vec3(ppos0), u8vec3(ppos1))))
                    return true;

            }

        }

        return false;
    }



} // cell
