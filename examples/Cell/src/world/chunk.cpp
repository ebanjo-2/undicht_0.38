#include <world/chunk.h>
#include <algorithm>

#include <math/cell_math.h>

namespace cell {

    using namespace undicht;
    using namespace tools;

    Chunk::Chunk() {

        setCell(Cell(u8vec3(0,0,0), u8vec3(255, 255, 255), VOID_CELL));
    }


    Chunk::~Chunk() {

    }

    const std::vector<Cell>& Chunk::getCells() const{
        /** dont edit these directly!! */

        return m_cells;
    }

    int Chunk::getCellCount() const {

        return m_cells.size();
    }

    /////////////////////////// adding / removing cells from m_cells /////////////////////////////

    int Chunk::addCell(const Cell& c) {
        /** recycles previously deleted Cell memory (if possible)
        * + adds the cell to the mini chunk system
        * so dont just use m_cells.push_back to add cells */


        int id;

        if(m_unused_cells.size()) {

            id = m_unused_cells.back();
            m_unused_cells.pop_back();

        } else {

            id = m_cells.size();
            m_cells.resize(id + 1 );
        }

        m_cells[id] = c;

        addToMiniChunks(id);

        return id;
    }

    void Chunk::remCell(int id) {

        remFromMiniChunks(id);

        m_unused_cells.push_back(id);
        m_cells[id] = Cell();
    }

    //////////////////////////////////// managing the mini chunks ////////////////////////////////////

    std::vector<int>& Chunk::getMiniChunk(int x, int y, int z) {
        // coords ranging from 0 to 15

        return m_mini_chunks[x][y][z];
    }

    std::vector<int>& Chunk::getMiniChunk(const u8vec3& pos) {
        // coords ranging from 0 to 15

        return m_mini_chunks[pos.x][pos.y][pos.z];
    }


    void Chunk::calcMiniChunks(int cell_id, u8vec3& chunk0, u8vec3& chunk1) {
        /** calculates which mini chunks c is part of */

        calcMiniChunks(m_cells[cell_id], chunk0, chunk1);
    }

    void Chunk::calcMiniChunks(const Cell& c, u8vec3& chunk0, u8vec3& chunk1) {

        u8vec3 pos0 = glm::min(c.m_pos0, c.m_pos1);
        u8vec3 pos1 = glm::max(c.m_pos0, c.m_pos1);

        chunk0 = pos0 / (unsigned char)16;
        chunk1 = pos1 / (unsigned char)16;
    }


    void Chunk::addToMiniChunks(int cell_id) {

        u8vec3 pos0, pos1;
        calcMiniChunks(cell_id, pos0, pos1);


        for(int x = pos0.x; x <= pos1.x; x++) {

            for(int y = pos0.y; y <= pos1.y; y++) {

                for(int z = pos0.z; z <= pos1.z; z++) {

                    std::vector<int>& mini_chunk = getMiniChunk(x,y,z);

                    mini_chunk.push_back(cell_id);

                }

            }

        }

    }

    void Chunk::remFromMiniChunks(int cell_id) {

        u8vec3 pos0, pos1;
        calcMiniChunks(cell_id, pos0, pos1);

        for(int x = pos0.x; x <= pos1.x; x++) {

            for(int y = pos0.y; y <= pos1.y; y++) {

                for(int z = pos0.z; z <= pos1.z; z++) {

                    std::vector<int>& mini_chunk = getMiniChunk(x,y,z);
                    std::vector<int>::iterator pos = std::find(mini_chunk.begin(), mini_chunk.end(), cell_id);

                    if(pos != mini_chunk.end()) {

                        mini_chunk.erase(pos);
                    } // else : something wasnt done corectly when initalizing the cell

                }

            }

        }
    }

    //////////////////////////////////// searching cells ////////////////////////////////////////


    int Chunk::getCellIDAt(const u8vec3& pos) {

        std::vector<int>& mini_chunk = getMiniChunk(pos / (unsigned char)16);

        for(int cell : mini_chunk)  {

            if(isPointInside(m_cells[cell], pos))
                return cell;

        }

        return -1;
    }


    void Chunk::getCellsInVolume(const Cell& volume, std::vector<int>& loadTo) {
        /** searches all of m_cells for cells that overlap with the volume */

        u8vec3 chunk0, chunk1;
        calcMiniChunks(volume, chunk0, chunk1);

        for(int x = chunk0.x; x <= chunk1.x; x++) {

            for(int y = chunk0.y; y <= chunk1.y; y++) {

                for(int z = chunk0.z; z <= chunk1.z; z++) {

                    std::vector<int>& mini_chunk = getMiniChunk(x,y,z);

                    getCellsInVolume(volume, loadTo, mini_chunk);

                }

            }

        }


        /*for(int i = 0; i < m_cells.size(); i++) {

            if(overlappingVolume(volume, m_cells[i])) {

                if(std::find(loadTo.begin(), loadTo.end(), i) == loadTo.end()) {

                    loadTo.push_back(i);
                }

            }

        }*/

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

        Cell ccopy = c;
        ccopy.m_visible_faces = calcVisibleFaces(c, total_cell_pool);

        addCell(ccopy);

        // modifying the cells that occupied the space before c
        for(int i : total_cell_pool) {

            subtractFromCell(i, getSharedVolume(c, m_cells[i]));
        }

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

        for(int i = 0; i < 6; i++) {

            if(new_cell_mask & FACE_MASK[i]) {

                addCell(new_cells[i]);
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

    ///////////////////// api for functions loading/generating cells /////////////////////


    void Chunk::readyForReInit(int num) {
        /** @warning only to be used before the chunk is constructed from a file / generator
        * @warning will breake the current chunk, editing will no longer work
        * @param will reserve enough space for num cells, so that while setting new cells
        * no resize of the cell buffer will be needed */

        m_cells.resize(num);
        m_unused_cells.clear();

        for(int i = 0; i < 4096; i++)
            m_mini_chunks[i / 256][(i / 16) % 16][i % 16].clear();

    }

    void Chunk::setCellBlind(const Cell& c, int offset) {
        /** @warning readyForReInit should be called before this function
        * wont check if the volume of the new cell was occupied before
        * usefull for initializing the chunk from data stored in files
        * which is already arranged without overlapping cells
        * will write the cell into the cell buffer at position 'offset'
        * without thinking about the cell that might have been there before*/


        if(offset >= m_cells.size()) {

            m_cells.resize(offset + 1);
        }

        m_cells[offset] = c;
        addToMiniChunks(offset);
    }



    ///////////////////////////// deciding which faces of cells cant be seen and therefor shouldnt be rendered /////////////////////////////

    unsigned char Chunk::calcVisibleFaces(const Cell& c, const std::vector<int>& cell_pool) {
        /** checks for every face if there are VOID_CELLS in front of it
        * if so, the face will be marked visible
        * the face will also be visible if its at the chunk edge
        * cells with material type VOID_CELL always have no visible faces */

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
        /** @return true, if there are empty cells within the volume
        * or if the volume goes outside the chunk range of [0,255]*/

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
