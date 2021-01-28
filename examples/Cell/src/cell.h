#ifndef CELL_H
#define CELL_H

struct Cell {

    unsigned short m_material; // short is 65536 numbers, should be more than enough

};

template<unsigned int SIZE>
struct CellChunk {
    /** a CellChunk describes a cuboid of cells with the same material
    *
    * @param SIZE: the number of bits needed to store the CellChunks position coordinates and size */

    // the material used by all sub-cells
    unsigned short m_material; // short is 65536 numbers, should be more than enough

unsigned int m_pos_x :
    SIZE;
unsigned int m_pos_y :
    SIZE;
unsigned int m_pos_z :
    SIZE;

unsigned int m_size_x :
    SIZE;
unsigned int m_size_y :
    SIZE;
unsigned int m_size_z :
    SIZE;

};





#endif // CELL_H
