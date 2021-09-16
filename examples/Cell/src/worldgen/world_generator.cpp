#include <worldgen/world_generator.h>

namespace cell {


    void WorldGenerator::initChunk(Chunk& c, const glm::ivec3& chunk_pos) {

        if(chunk_pos.y == 0) {

            c.setCell(Cell(u8vec3(0,0,0), u8vec3(255, 20, 255), 1));
        }

        if(chunk_pos.y < 0) {

            c.setCell(Cell(u8vec3(0,0,0), u8vec3(255, 255, 255), 0));
        }

    }


} // cell
