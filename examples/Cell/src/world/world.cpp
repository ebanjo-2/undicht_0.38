#include <world/world.h>
#include <math/cell_math.h>

namespace cell {

    ///////////////////////////// controlling world generation //////////////////////////////

    void World::setWorldGenerator(const WorldGenerator& gen) {

        m_generator = gen;
    }

    ///////////////////////////// controlling the loaded chunks /////////////////////////////


    void World::loadChunks(const glm::ivec3& origin, int distance) {
        /** the chunks around the origin will be loaded
        * (you may want to use the players position as the loaded worlds origin )*/

        m_dst = distance;

        std::vector<glm::ivec3> chunk_positions = calcLoadedChunkPositions(origin);
        std::vector<glm::ivec3> chunks_to_load = calcChunksToLoad(chunk_positions);

        // unloading the old chunks
        //unloadChunks(chunk_positions);

        // loading the chunks that need to be loaded
        for(const glm::ivec3& pos : chunks_to_load) {

            m_loaded_chunks.emplace_back(Chunk());
            m_chunk_positions.push_back(pos);

            m_generator.initChunk(m_loaded_chunks.back(), pos);

        }


    }


    std::vector<glm::ivec3> World::calcLoadedChunkPositions(const glm::ivec3& origin) {

        std::vector<glm::ivec3> positions;

        glm::ivec3 origin_chunk = getChunkPos(origin);

        for(int x = -1 * m_dst; x <= m_dst; x++) {

            for(int y = -1 * m_dst; y <= m_dst; y++) {

                for(int z = -1 * m_dst; z <= m_dst; z++) {

                    positions.push_back(origin_chunk + glm::ivec3(x,y,z) * 255);

                }

            }

        }

        return positions;
    }

    std::vector<glm::ivec3> World::calcChunksToLoad(const std::vector<glm::ivec3>& new_chunks) {

        std::vector<glm::ivec3> chunks_to_load;

        for(const glm::ivec3& pos : new_chunks) {

            bool found = false;

            // std::find doesnt work ?
            for(const glm::ivec3& old_pos : m_chunk_positions) {

                if(old_pos == pos) {
                    found = true;
                    break;
                }

            }

            if(!found) {
                chunks_to_load.push_back(pos);


            }

        }

        return chunks_to_load;
    }

    void World::unloadChunks(const std::vector<glm::ivec3>& new_chunks) {
        /** unloads the chunks that are not part of the new_chunks */

        for(int i = 0; i < m_chunk_positions.size(); i++) {

            const glm::ivec3& old_pos = m_chunk_positions[i];

            bool found = false;

            for(const glm::ivec3& pos : new_chunks) {

                if(old_pos == pos) {
                    found = true;
                    break;
                }

            }

            if(!found) {
                // old_pos is not part of the new chunks

                m_loaded_chunks.erase(m_loaded_chunks.begin() + i);
                m_chunk_positions.erase(m_chunk_positions.begin() + i);

            }

        }

    }

    ////////////////////////////////// editing the world //////////////////////////////////

    glm::ivec3 World::getChunkPos(const glm::ivec3& pos) {
        /** @return the position of the chunk in which pos is located*/

        glm::ivec3 chunk_pos = (pos / 255) * 255; // integers!!!

        if(pos.x < 0)
            chunk_pos.x -= 255;

        if(pos.y < 0)
            chunk_pos.y -= 255;

        if(pos.z < 0)
            chunk_pos.z -= 255;

        return chunk_pos;
    }

    Chunk& World::getChunk(const glm::ivec3& chunk_pos) {

        for(int i = 0; i < m_loaded_chunks.size(); i++) {

            if(m_chunk_positions[i] == chunk_pos)
                return m_loaded_chunks[i];

        }

        return m_loaded_chunks[0];
    }

    void World::setCell(const TCell<int>& c) {
        /** parts of the cell that are outside the loaded chunks will be ignored */

        for(int i = 0; i < m_loaded_chunks.size(); i++) {

            glm::ivec3 chunk_pos = m_chunk_positions[i];
            TCell<int> chunk_cell(chunk_pos, chunk_pos + glm::ivec3(255));

            if(overlappingVolume(chunk_cell, c)) {

                TCell<int> isub_cell = getSharedVolume(chunk_cell, c);

                Cell sub_cell = Cell(u8vec3(isub_cell.m_pos0 - chunk_pos), u8vec3(isub_cell.m_pos1 - chunk_pos), c.m_material);
                m_loaded_chunks[i].setCell(sub_cell);
            }


        }

    }


} // cell
