#ifndef CELL_H
#define CELL_H

#include <glm/glm.hpp>
#include <iostream>

namespace cell {


    const extern unsigned char YP;
    const extern unsigned char YN;
    const extern unsigned char XP;
    const extern unsigned char XN;
    const extern unsigned char ZP;
    const extern unsigned char ZN;

    const extern unsigned char FACE_MASK[];
    const extern unsigned char OPPOSING_FACE_MASK[];



    const extern unsigned short VOID_CELL;

    // create the type that stores 3D coordinates of a cell within the 255 * 255 * 255 volume
    typedef glm::detail::tvec3<unsigned char> u8vec3;

    class Cell {
            // size of one cell should be 3 bytes (pos0) + 3 bytes (pos1) + 2 bytes (material) = 8 bytes
        public:

            u8vec3 m_pos0 = u8vec3(0,0,0);
            u8vec3 m_pos1 = u8vec3(0,0,0);

            // cells with material id 65535 (-1, highest ushort) are invisible
            unsigned short m_material = 0;

            unsigned char m_visible_faces = 0x00;

        public:

            Cell();
            Cell(const u8vec3& pos0, const u8vec3& pos1, unsigned short mat = 0, unsigned char visible_faces = 0x00);
            // no virtual destr cause that would increase the size of the class

            void setMaterial(unsigned short mat);
            unsigned short getMaterial();


    };

    std::ostream& operator << (std::ostream& out, const Cell& c);

} // cell

#endif // CELL_H
