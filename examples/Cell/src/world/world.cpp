#include <world/world.h>
#include <math/cell_math.h>
#include <algorithm>

#include <world/chunk_optimization.h>

namespace cell {

    using namespace undicht;
    using namespace tools;


    World::World() {

        m_world_file.open("res/first_world.und");

    }

    World::~World() {

        if(m_opt_thread)
            delete m_opt_thread; // will wait for the thread to finish


        if(m_temp_chunk)
            delete m_temp_chunk;

        unloadChunks({});

    }

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
        unloadChunks(chunk_positions);

        // loading the chunks that need to be loaded
        for(const glm::ivec3& pos : chunks_to_load) {

            m_loaded_chunks.emplace_back(WorldChunk(pos));

            if(!m_world_file.readChunk(m_loaded_chunks.back(), getChunkId(pos / 255)))
                m_generator.initChunk(m_loaded_chunks.back(), pos);

            m_loaded_chunks.back().updateDrawBuffer();

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
            for(const WorldChunk& old_chunk : m_loaded_chunks) {

                if(old_chunk.getOrigin() == pos) {
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

        std::vector<int> chunks_to_unload;

        for(int i = 0; i < m_loaded_chunks.size(); i++) {

            const glm::ivec3& old_pos = m_loaded_chunks[i].getOrigin();

            bool found = false;

            for(const glm::ivec3& pos : new_chunks) {

                if(old_pos == pos) {
                    found = true;
                    break;
                }

            }

            if(!found) {
                // old_pos is not part of the new chunks

                // saving the chunk to the file
                const WorldChunk& c = m_loaded_chunks[i];
                m_world_file.writeChunk(c, getChunkId(old_pos / 255));

                chunks_to_unload.push_back(i);

            }

        }

        // sorting the array in reverse order (biggest id fist)
        std::sort(chunks_to_unload.rbegin(), chunks_to_unload.rend());

        for(int i : chunks_to_unload) {

            m_loaded_chunks.erase(m_loaded_chunks.begin() + i);
        }

    }

    ////////////////////////// optimizing chunks that have been edited //////////////////////////

    void World::optChunks() {


        if(m_opt_thread && m_opt_thread->hasFinished()) {
            // a chunk has been optimized

            if(m_loaded_chunks[m_opt_chunk_id].getOrigin() == m_temp_chunk->getOrigin()) {


                if(m_loaded_chunks[m_opt_chunk_id].getOptNeed() == m_last_opt_need) {
                    // else: the chunk has been changed; applying the optimized version would revert those changes

                    m_loaded_chunks[m_opt_chunk_id] = *m_temp_chunk;
                    m_loaded_chunks[m_opt_chunk_id].updateDrawBuffer();
                    m_loaded_chunks[m_opt_chunk_id].markAsOptimzed();

                }

            }

            m_opt_chunk_id = -1;

            delete m_opt_thread;
            m_opt_thread = 0;

            delete m_temp_chunk;
            m_temp_chunk = 0;

        }

        if(!m_opt_thread) {
            // finding a new chunk to optimize

            int c = 0; // the chunk with the highest need for optimization

            for(int i = 1; i < m_loaded_chunks.size(); i++) {

                if(m_loaded_chunks[i].getOptNeed() > m_loaded_chunks[c].getOptNeed()) {

                    c = i;
                }

            }

            // optimizing the chunk with the highest opt need
            if(m_loaded_chunks[c].getOptNeed()) {

                m_opt_chunk_id = c;
                m_temp_chunk = new WorldChunk(m_loaded_chunks[c].getOrigin());

                m_opt_thread = new Thread(optimizeChunk, m_loaded_chunks[c], m_temp_chunk);

                // to check if the chunk got changed during optimization
                m_last_opt_need = m_loaded_chunks[c].getOptNeed();

            }

        }

    }

    /////////////////////////////////////// collision detection ///////////////////////////////////////



    Cell World::getCellAt(const glm::ivec3& pos) {
        /** @return undefined, if the requested position is outside the loaded world */

        WorldChunk& cchunk = getChunk(getChunkPos(pos));

        return cchunk.getCellAt(pos);
    }

    std::vector<Cell> World::getCellsAt(glm::ivec3& pos, const glm::ivec3& siz) {
        /** @return the cells within the described volume
        * siz can not be larger than 255 * 255 * 255 */

        // the corners of the described volume can be in 8 different chunks at max
        glm::ivec3 corner0 = offset;
        glm::ivec3 corner1 = offset + siz.;
        glm::ivec3 corner2 = offset;
        glm::ivec3 corner3 = offset + glm::ivec3(volume.);


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

    WorldChunk& World::getChunk(const glm::ivec3& chunk_pos) {

        for(int i = 0; i < m_loaded_chunks.size(); i++) {

            if(m_loaded_chunks[i].getOrigin() == chunk_pos)
                return m_loaded_chunks[i];

        }

        return m_loaded_chunks[0];
    }

    void World::setCell(const TCell<int>& c) {
        /** parts of the cell that are outside the loaded chunks will be ignored */

        for(int i = 0; i < m_loaded_chunks.size(); i++) {

            glm::ivec3 chunk_pos = m_loaded_chunks[i].getOrigin();
            TCell<int> chunk_cell(chunk_pos, chunk_pos + glm::ivec3(255));

            if(overlappingVolume(chunk_cell, c)) {

                TCell<int> isub_cell = getSharedVolume(chunk_cell, c);

                Cell sub_cell = Cell(u8vec3(isub_cell.m_pos0 - chunk_pos), u8vec3(isub_cell.m_pos1 - chunk_pos), c.m_material);
                m_loaded_chunks[i].setCell(sub_cell);
            }


        }

    }

    int World::getChunkId(const glm::ivec3& chunk_pos) {

        glm::ivec3 relative = chunk_pos + glm::ivec3(8);

        return relative.x * 256 + relative.y * 16 + relative.z;
    }

} // cell
