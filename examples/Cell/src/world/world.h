#ifndef WORLD_H
#define WORLD_H

#include <world/chunk.h>
#include <worldgen/world_generator.h>
#include <worldgen/world_file.h>

namespace cell {

    class World {

        public:

            WorldFile m_world_file;

        public:

            std::vector<Chunk> m_loaded_chunks;

            /** chunks start every 255 cells */
            std::vector<glm::ivec3> m_chunk_positions;

            WorldGenerator m_generator;

            glm::ivec3 m_origin_chunk; // the origin chunk of the loaded world

            // number of chunks to load in every direction of the origin chunk
            int m_dst = 1;

        public:

            World();
            virtual ~World();

        public:
            // controlling world generation

            void setWorldGenerator(const WorldGenerator& gen);

        public:
            // controlling the loaded chunks

            /** the chunks around the origin will be loaded
            * (you may want to use the players position as the loaded worlds origin )
            * @param distance number of chunks to load in every direction of the origin chunk */
            void loadChunks(const glm::ivec3& origin, int distance = 1);

            std::vector<glm::ivec3> calcLoadedChunkPositions(const glm::ivec3& origin);

            std::vector<glm::ivec3> calcChunksToLoad(const std::vector<glm::ivec3>& new_chunks);

            /** unloads the chunks that are not part of the new_chunks */
            void unloadChunks(const std::vector<glm::ivec3>& new_chunks);

        public:
            // editing the world

            /** @return the position of the chunk in which pos is located*/
            glm::ivec3 getChunkPos(const glm::ivec3& pos);

            Chunk& getChunk(const glm::ivec3& chunk_pos);

            /** parts of the cell that are outside the loaded chunks will be ignored */
            void setCell(const TCell<int>& c);


    };


} // cell

#endif // WORLD_H
