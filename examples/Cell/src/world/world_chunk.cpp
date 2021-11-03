#include "world/world_chunk.h"

namespace cell {

    WorldChunk::WorldChunk() {

    }


    WorldChunk::WorldChunk(const glm::ivec3& origin) {

        setOrigin(origin);
    }


    int WorldChunk::getOptNeed() {
        /** @return a rough estimation of how much a optimization of the chunk is needed
        * 0 can mean that there is no need, the heigher the return,
        * the more cells were added to the chunk and the heigher the need for optimization */

        return getCellCount() - m_last_optimized_cell_count;
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

} // cell
