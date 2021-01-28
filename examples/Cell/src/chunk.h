#ifndef CHUNK_H
#define CHUNK_H


#include <cell.h>
#include <vector>
#include <array>

#include <math.h>

template <unsigned short SIZE>
class CompactChunk {
        /** @brief a chunk describes a cube containing cells
        *
        * CompactChunks store those cells in a memory efficient way,
        * by combining neighbouring cells with the same material into cuboids
        *
        * @param chunks have different SIZEs, the side length of the cube is 2 to the power of SIZE
        * (i.e. : SIZE = 1, so cube is 2*2*2, SIZE = 2, so cube is 4*4*4 */
    public:

        std::vector<CellChunk<SIZE>> m_cells;

    public:
        // interacting with cells
        // you cannot change the cells stored in a compact chunk, change to default chunk instead

        /** @return the material id of the sub-cell at the requested position */
        unsigned short getMaterial(unsigned int x, unsigned int y, unsigned int z);

        /** @return the cell that covers the requested position */
        const Cell* getCell(unsigned int x, unsigned int y, unsigned int z);

        CompactChunk() {}
        virtual ~CompactChunk() {}

};


template <unsigned short SIZE>
class Chunk {
        /** @brief a chunk describes a cube containing cells
        *
        * @param chunks have different SIZEs, the side length of the cube is 2 to the power of SIZE
        * (i.e. : SIZE = 1, so cube is 2*2*2, SIZE = 2, so cube is 4*4*4 */
    public:

        // (1<<SIZE) is just a way of calculating 2 to the power of SIZE
        std::array<std::array<std::array<Cell, (1<<SIZE)>, (1<<SIZE)>, (1<<SIZE)> m_cells;

    public:
        // interacting with cells
        // you cannot change the cells stored in a compact chunk, change to default chunk instead

        /** @return the material id of the sub-cell at the requested position */
        unsigned short getMaterial(unsigned int x, unsigned int y, unsigned int z);

        /** @return the cell that covers the requested position */
        const Cell* getCell(unsigned int x, unsigned int y, unsigned int z);

        Chunk() {}
        virtual ~Chunk() {}

};


// functions to convert between compact and normal chunks

/** @return true, if the compact chunk is actually smaller in size */
template<unsigned int SIZE>
bool compressChunk(const Chunk<SIZE>& c, CompactChunk<SIZE>& cc) {
    // to be done

    unsigned int expander = 0;




    if(sizeof(c) < cc) {
        return false;
    } else {
        return true;
    }
}

/** @return true, if the compact chunk is actually smaller in size */
template<unsigned int SIZE>
void expandChunk(const CompactChunk<SIZE>& cc, Chunk<SIZE>& c) {
    // to be done

    for(CellChunk<SIZE>& cellc : cc.m_cells) {

        for(int x = 0; x < cellc.m_size_x; x++) {
            for(int y = 0; y < cellc.m_size_y; y++) {
                for(int z = 0; z < cellc.m_size_z; z++) {

                    c.m_cells[cellc.m_pos_x + x][cellc.m_pos_y + y][cellc.m_pos_z + z].m_material = cellc.m_material;

                }
            }
        }

    }

}


/** compares the cells of the chunk, if they are equal, a 0 cell is going to be placed in the result chunk
* if they are not equal, the cell is going to be copied from the second chunk */
template<unsigned int SIZE>
void compareChunks(const Chunk<SIZE>& c1, const Chunk<SIZE>& c2, Chunk<SIZE>& result) {

    unsigned int chunk_size = 1<<SIZE;


    for(int x = 0; x < chunk_size; x++) {
        for(int y = 0; y < chunk_size; y++) {
            for(int z = 0; z < chunk_size; z++) {

                if(c1.m_cells[x][y][z] == c2.m_cells[x][y][z]) {

                    result.m_cells[x][y][z].m_material = 0;
                } else {

                    result.m_cells[x][y][z] = c2.m_cells[x][y][z];
                }

            }
        }
    }

}



#endif // CHUNK_H
