#ifndef CHUNK_OPTIMIZATION_H
#define CHUNK_OPTIMIZATION_H

#include <world/chunk.h>

namespace cell {

    /** tries to minimize the number of cells in the chunk */
    void optimizeChunk(const Chunk* old, Chunk* optimized);


} // cell

#endif // CHUNK_OPTIMIZATION_H
