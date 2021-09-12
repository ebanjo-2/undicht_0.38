#include <world/chunk_optimization.h>


namespace cell {

    // stores the material of every position within a chunk
    std::vector<unsigned short> cell_mat_buffer(255 * 255 * 255);
    std::vector<bool> cell_set_buffer(255 * 255 * 255);

    void loadChunk(const Chunk& chunk);

    Cell findCell(const u8vec3& pos0);

    unsigned char calcVisibleFaces(const Cell& c) ;

    void markCellAsSet(const Cell& c);

    /** @return true, if the volume is completely filled with that material */
    bool sameMaterial(const u8vec3& pos0, const u8vec3& pos1, unsigned short mat);

    bool containsVoid(const glm::ivec3& pos0, const glm::ivec3& pos1);




    void optimizeChunk(Chunk& chunk) {
        /** tries to minimize the number of cells in the chunk */

        loadChunk(chunk);

        chunk.m_cells.clear();
        chunk.m_unused_cells.clear();

        for(int x = 0; x < 255; x++) {

            for(int y = 0; y < 255; y++) {

                for(int z = 0; z < 255; z++) {

                    if(!cell_set_buffer[x * 255 * 255 + y * 255 + z]) {

                        Cell c = findCell(u8vec3(x,y,z));
                        c.m_visible_faces = calcVisibleFaces(c);

                        chunk.m_cells.push_back(c);

                        markCellAsSet(c);

                    }
                }

            }

        }

    }

    void loadChunk(const Chunk& chunk) {

        for(const Cell& c : chunk.m_cells) {

            u8vec3 pos0 = glm::min(c.m_pos0, c.m_pos1);
            u8vec3 pos1 = glm::max(c.m_pos0, c.m_pos1);

            for(int x = pos0.x; x < pos1.x; x++) {

                for(int y = pos0.y; y < pos1.y; y++) {

                    std::vector<unsigned short>::iterator i1 = cell_mat_buffer.begin() + (x * 255 * 255) + (y * 255) + pos0.z;
                    std::vector<unsigned short>::iterator i2 = cell_mat_buffer.begin() + (x * 255 * 255) + (y * 255) + pos1.z;


                    std::fill(i1, i2, c.m_material);

                }

            }

        }

        std::fill(cell_set_buffer.begin(), cell_set_buffer.end(), false);

    }

    Cell findCell(const u8vec3& pos0) {

        bool search_x = true;
        bool search_y = true;
        bool search_z = true;

        u8vec3 pos1 = pos0 + u8vec3(1,1,1);

        unsigned short mat = cell_mat_buffer[pos0.x * 255 * 255 + pos0.y * 255 + pos0.z];

        while(search_x || search_y || search_z) {

            if(search_x && (pos1.x != 255)) {
                // searching in +x direction
                if(sameMaterial(u8vec3(pos1.x, pos0.y, pos0.z), u8vec3(pos1.x + 1, pos1.y, pos1.z), mat)) {

                    pos1.x += 1;
                } else {

                    search_x = false;
                }

            } else {

                search_x = false;
            }

            if(search_y && (pos1.y != 255)) {
                // searching in +y direction
                if(sameMaterial(u8vec3(pos0.x, pos1.y, pos0.z), u8vec3(pos1.x, pos1.y + 1, pos1.z), mat)) {

                    pos1.y += 1;
                } else {

                    search_y = false;
                }

            } else {

                search_y = false;
            }

            if(search_z && (pos1.z != 255)) {
                // searching in +z direction
                if(sameMaterial(u8vec3(pos0.x, pos0.y, pos1.z), u8vec3(pos1.x, pos1.y, pos1.z + 1), mat)) {

                    pos1.z += 1;
                } else {

                    search_z = false;
                }

            } else {

                search_z = false;
            }

        }

        return Cell(pos0, pos1, mat);
    }

    unsigned char calcVisibleFaces(const Cell& c) {

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

            if(containsVoid(faces[i * 2 + 0], faces[i * 2 + 1]))
                visible_faces |= FACE_MASK[i];

        }

        return visible_faces;
    }


    void markCellAsSet(const Cell& c) {

        u8vec3 pos0 = glm::min(c.m_pos0, c.m_pos1);
        u8vec3 pos1 = glm::max(c.m_pos0, c.m_pos1);

        for(int x = pos0.x; x < pos1.x; x++) {

            for(int y = pos0.y; y < pos1.y; y++) {

                std::vector<bool>::iterator i1 = cell_set_buffer.begin() + (x * 255 * 255) + (y * 255) + pos0.z;
                std::vector<bool>::iterator i2 = cell_set_buffer.begin() + (x * 255 * 255) + (y * 255) + pos1.z;


                std::fill(i1, i2, true);

            }

        }
    }


    bool sameMaterial(const u8vec3& pos0, const u8vec3& pos1, unsigned short mat) {
        /** if the volume is completely filled with that material */

        for(int x = pos0.x; x < pos1.x; x++) {

            for(int y = pos0.y; y < pos1.y; y++) {

                for(int z = pos0.z; z < pos1.z; z++) {

                    if(cell_mat_buffer[x * 255 * 255 + y * 255 + z] != mat)
                        return false;

                    if(cell_set_buffer[x * 255 * 255 + y * 255 + z])
                        return false;
                }
            }
        }

        return true;
    }


    bool containsVoid(const glm::ivec3& pos0, const glm::ivec3& pos1) {

        if(pos0.x < 0 || pos0.y < 0 || pos0.z < 0) return true;
        if(pos1.x > 255 || pos1.y > 255 || pos1.z > 255) return true;


        for(int x = pos0.x; x < pos1.x; x++) {

            for(int y = pos0.y; y < pos1.y; y++) {

                for(int z = pos0.z; z < pos1.z; z++) {

                    if(cell_mat_buffer[x * 255 * 255 + y * 255 + z] == VOID_CELL)
                        return true;

                }
            }
        }

        return false;
    }


} // cell
