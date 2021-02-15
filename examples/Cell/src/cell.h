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

	void operator= (const std::array<unsigned int, 7>& a) {

		m_pos_x = a[0];
		m_pos_y = a[1];
		m_pos_z = a[2];

		m_size_x = a[3];
		m_size_y = a[4];
		m_size_z = a[5];
	
		m_material = a[6];
	}
};





#endif // CELL_H
