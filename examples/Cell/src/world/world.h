#ifndef WORLD_H
#define WORLD_H

#include <world/world_chunk.h>
#include <worldgen/world_generator.h>
#include <worldgen/world_file.h>

#include <undicht_thread.h>


namespace cell {

    class World {

        public:

            WorldFile m_world_file;

            WorldGenerator m_generator;

        public:

            std::vector<WorldChunk> m_loaded_chunks;


        public:

            glm::ivec3 m_origin_chunk; // the origin chunk of the loaded world

            // number of chunks to load in every direction of the origin chunk
            int m_dst = 1;

        public:
            // optimizing chunks

            undicht::tools::Thread* m_opt_thread = 0;
            WorldChunk* m_temp_chunk = 0;
            int m_opt_chunk_id = -1; // the chunk that currently gets optimized
            int m_last_opt_need = 0;

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
            // optimizing chunks that have been edited

            void optChunks();

        public:
            // collision detection

            /** @return undefined, if the requested position is outside the loaded world */
            Cell getCellAt(const glm::ivec3& pos);

            /** @return the cells within the described volume
            * siz can not be larger than 255 * 255 * 255 */
            std::vector<Cell> getCellsAt(glm::ivec3& pos, const glm::ivec3& siz);


        public:
            // editing the world

            /** @return the position of the chunk in which pos is located*/
            glm::ivec3 getChunkPos(const glm::ivec3& pos);

            WorldChunk& getChunk(const glm::ivec3& chunk_pos);

            /** parts of the cell that are outside the loaded chunks will be ignored */
            void setCell(const TCell<int>& c);

        public:

            int getChunkId(const glm::ivec3& chunk_pos);


    };


} // cell

#endif // WORLD_H
