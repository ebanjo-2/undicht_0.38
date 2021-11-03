#ifndef CHUNK_OPTIMIZATION_H
#define CHUNK_OPTIMIZATION_H

#include <world/chunk.h>
#include <array>

namespace cell {

    class ChunkOptimizer {

        public:

            // stores the material of every position within a chunk
            std::array<unsigned short, 255 * 255 * 255> m_cell_mat_buffer;
            std::array<bool, 255 * 255 * 255> m_cell_set_buffer;

        public:

            /** tries to minimize the number of cells in the chunk */
            void optimizeChunk(Chunk old_chunk, Chunk* optimized);

        protected:

            void loadChunk(const Chunk& chunk);

            Cell findCell(const u8vec3& pos0);

            unsigned char calcVisibleFaces(const Cell& c) ;

            void markCellAsSet(const Cell& c);

            /** @return true, if the volume is completely filled with that material */
            bool sameMaterial(const u8vec3& pos0, const u8vec3& pos1, unsigned short mat);

            bool containsVoid(const glm::ivec3& pos0, const glm::ivec3& pos1);


    };

    /** tries to minimize the number of cells in the chunk */
    void optimizeChunk(Chunk old, Chunk* optimized);


} // cell

#endif // CHUNK_OPTIMIZATION_H
