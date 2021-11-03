#ifndef WORLD_CHUNK_H
#define WORLD_CHUNK_H


#include <world/chunk.h>

namespace cell {

    class WorldChunk : public Chunk {
            // contains information about a chunk placed into the world
            // such as its offset and changes made to it

        protected:

            // the world position of the 0,0,0 block of the chunk
            glm::ivec3 m_origin;

            int m_last_optimized_cell_count = 0;

        public:

            /** @return a rough estimation of how much a optimization of the chunk is needed
            * 0 can mean that there is no need, the heigher the return,
            * the more cells were added to the chunk and the heigher the need for optimization */
            int getOptNeed();

            /** tells the chunk that its just been optimized
            * getOptNeed() will now return 0 until changes are made to the chunk*/
            void markAsOptimzed();

        public:

            const glm::ivec3& getOrigin() const;
            void setOrigin(const glm::ivec3& origin);


        public:

            WorldChunk();
            WorldChunk(const glm::ivec3& origin);


    };


} // cell

#endif // WORLD_CHUNK_H
