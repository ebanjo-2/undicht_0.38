#include <world/chunk.h>
#include <algorithm>
#include <geometry/geometry.h>

#include <math/cell_math.h>

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

    }


    Chunk::~Chunk() {

    }


    //////////////////////////////////////// managing the edit cells //////////////////////////////////////


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

    int Chunk::getCellRef(const u8vec3& pos) const {

        return layer[pos.y].row[pos.z].cell[pos.x];
    }

    int Chunk::getCellRef(unsigned char x, unsigned char y, unsigned char z) const {

        return layer[y].row[z].cell[x];
    }

    //////////////////////////////////// editing cells //////////////////////////////////////////

    const Cell* Chunk::getCell(const u8vec3& pos) const {

        int cell_ref = getCellRef(pos);

        if(cell_ref == -1) return 0;

        return &m_cells[cell_ref];
    }

    void Chunk::getCells(const u8vec3& ppos0, const u8vec3& ppos1, std::vector<int>& loadTo) const {
        /** finds all the cells in the volume of the cuboid described by pos0 and pos1 */

       // if(getVolume(Cell(ppos0, ppos1)) < m_cells.size()) {

            //getCellsBySearchingVol(ppos0, ppos1, loadTo);
        //} else {

            getCellsBySearchingAll(ppos0, ppos1, loadTo);
        //}

    }

    void Chunk::getCellsBySearchingAll(const u8vec3& ppos0, const u8vec3& ppos1, std::vector<int>& loadTo) const {

        Cell vol(ppos0, ppos1);

        for(int i = 0; i < m_cells.size(); i++) {

            if(overlappingVolume(vol, m_cells[i])) {

                if(std::find(loadTo.begin(), loadTo.end(), i) == loadTo.end()) {

                    loadTo.push_back(i);
                }

            }

        }


    }

    void Chunk::getCellsBySearchingVol(const u8vec3& ppos0, const u8vec3& ppos1, std::vector<int>& loadTo) const {

        u8vec3 pos0 = glm::min(ppos0, ppos1);
        u8vec3 pos1 = glm::max(ppos0, ppos1);

        for(unsigned char x = pos0.x; x < pos1.x; x++) {

            for(unsigned char y = pos0.y; y < pos1.y; y++) {

                //std::cout << "y  " << (int)y << "\n";

                for(unsigned char z = pos0.z; z < pos1.z; z++) {

                    //std::cout << "z  " << (int)z << "\n";

                    int cell_ref = getCellRef(x,y,z);

                    if(cell_ref != -1) {

                        if(std::find(loadTo.begin(), loadTo.end(), cell_ref) == loadTo.end()) {

                            loadTo.push_back(cell_ref);
                        }
                        // the referenced cell seems to be wrong in very few cases
                        z = glm::max(m_cells[cell_ref].m_pos0.z, m_cells[cell_ref].m_pos1.z); // skipping the rest of the cell
                    }

                }

            }

        }

    }


    /** sets multiple cells */
    void Chunk::setCells(const std::vector<Cell>& cells) {

        for(const Cell& c : cells) {

            setCell(c);
        }
    }

    void Chunk::setCell(const Cell& c) {

        if(!layer.size()) {
            // edit cells are not initialized
            setCellBlind(c);
            return;
        }

        std::vector<int> affected_cells; // the cells affected by setting c
        getCells(c.m_pos0, c.m_pos1, affected_cells);

        for(int i : affected_cells) {

            if(!overlappingVolume(c, m_cells[i])) std::cout << "wtf" << "\n";

            Cell shared_cell = getSharedVolume(c, m_cells[i]);
            std::array<Cell, 6> split_cells;
            subtractCells(m_cells[i], shared_cell, split_cells);

            setCellBlind(Cell(), i); // removing the cell that got split

            for(Cell& split : split_cells) {

                if(getVolume(split))
                    setCellBlind(split);

            }

        }

        setCellBlind(c);
    }


    void Chunk::setCellBlind(const Cell& c, int id) {
        /** adding the cell to the chunks array of cells
        * without checking for existing cells in the new cells space */

        int cell_id = id == -1 ? m_cells.size() : id; // id of the new cell

        setCellRef(c, cell_id);

        if(cell_id >= m_cells.size()) m_cells.resize(cell_id + 1);
        if(cell_id >= m_visible_faces.size()) m_visible_faces.resize(cell_id + 1);

        m_visible_faces[cell_id] = calcVisibleFaces(c);
        m_cells[cell_id] = c;

        updateDrawBuffer(cell_id);
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

        unsigned char visible_faces = m_visible_faces[id];



        unsigned char data[] = {

            c.m_pos0.x, c.m_pos0.y, c.m_pos0.z,
            c.m_pos1.x, c.m_pos1.y, c.m_pos1.z,
            mat1, mat0, // i was pretty sure it had to be mat0, mat1    but only this works (even with numbers > 255)
            visible_faces
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

    unsigned char Chunk::calcVisibleFaces(const Cell& c) {

        if(!layer.size()) return 255; // edit cells not initialized

        glm::ivec3 pos0 = (glm::ivec3)glm::min(c.m_pos0, c.m_pos1);
        glm::ivec3 pos1 = (glm::ivec3)glm::max(c.m_pos0, c.m_pos1);

        // vertices of all the faces along which the neighbouring cells need to be checked
        std::array<glm::ivec3, 12> faces = {
            glm::ivec3(pos0.x, pos1.y + 1, pos0.z), glm::ivec3(pos1.x, pos1.y + 2, pos1.z), // +y
            glm::ivec3(pos0.x, pos0.y - 1, pos0.z), glm::ivec3(pos1.x, pos0.y - 0, pos1.z), // -y
            glm::ivec3(pos1.x + 1, pos0.y, pos0.z), glm::ivec3(pos1.x + 2, pos1.y, pos1.z), // +x
            glm::ivec3(pos0.x - 1, pos0.y, pos0.z), glm::ivec3(pos0.x - 0, pos1.y, pos1.z), // -x
            glm::ivec3(pos0.x, pos0.y, pos1.z + 1), glm::ivec3(pos1.x, pos1.y, pos1.z + 2), // +z
            glm::ivec3(pos0.x, pos0.y, pos0.z - 1), glm::ivec3(pos1.x, pos1.y, pos0.z - 0), // -z
        };

        unsigned char visible_faces = 0x00;

        for(int i = 0; i < 6; i++) {

            for(int x = faces[i * 2 + 0].x; x < faces[i * 2 + 1].x; x++) {

                for(int y = faces[i * 2 + 0].y; y < faces[i * 2 + 1].y; y++) {

                    for(int z = faces[i * 2 + 0].z; z < faces[i * 2 + 1].z; z++) {

                        if(!isCellSolid(glm::ivec3(x,y,z))) {
                            // there is no solid cell at that position
                            visible_faces = visible_faces | FACE_MASK[i];
                            break;
                        }

                    }

                    if(FACE_MASK[i] & visible_faces) break; // face is already marked visible
                }

                if(FACE_MASK[i] & visible_faces) break; // skipping to the next face
            }

        }

        return visible_faces;
    }

    /*void Chunk::updateVisibleFaces() {

        m_visible_faces.resize(m_cells.size());

        for(int i = 0; i < m_cells.size(); i++) {

            Cell& c = m_cells[i];
            m_visible_faces[i] = calcVisibleFaces(c);

        }

    }*/


    bool Chunk::isCellSolid(const glm::ivec3& pos) {
        /** @return true, if the cell ref at pos is not -1
        * note that this function takes an ivec3, which means the values can exceed the range of 0 to 255
        * (i.e. when checking on a chunks edge)
        * in which case this function will always return false
        * it may be a good idea to actually take other chunks into account in the future */

        if((pos.x < 0) || (pos.x > 255)) return false;
        if((pos.y < 0) || (pos.y > 255)) return false;
        if((pos.z < 0) || (pos.z > 255)) return false;

        return (layer[pos.y].row[pos.z].cell[pos.x] != -1);
    }




} // cell
