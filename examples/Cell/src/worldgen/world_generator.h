#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include <world/chunk.h>

namespace cell {

class WorldGenerator {

public:

    virtual void initChunk(Chunk& c, const glm::ivec3& chunk_pos);


};


} // cell

#endif // WORLD_GENERATOR_H
