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

    template<typename T>
    class TCell {
            // size of one cell should be 3 bytes (pos0) + 3 bytes (pos1) + 2 bytes (material) = 8 bytes
        public:

            glm::detail::tvec3<T> m_pos0 = glm::detail::tvec3<T>(0,0,0);
            glm::detail::tvec3<T> m_pos1 = glm::detail::tvec3<T>(0,0,0);

            // cells with material id 65535 (-1, highest ushort) are invisible
            unsigned short m_material = 0;

            unsigned char m_visible_faces = 0x00;

        public:

            TCell() {};
            TCell(const glm::detail::tvec3<T>& pos0, const glm::detail::tvec3<T>& pos1, unsigned short mat = 0, unsigned char visible_faces = 0x00)  {

                m_pos0 = pos0;
                m_pos1 = pos1;
                m_material = mat;
                m_visible_faces = visible_faces;

            };

            // no virtual destr cause that would increase the size of the class

            void setMaterial(unsigned short mat) {

                m_material = mat;
            };

            unsigned short getMaterial() {

                return m_material;
            };


    };

    typedef TCell<unsigned char> Cell; // default cell

    std::ostream& operator << (std::ostream& out, const Cell& c);

} // cell

#endif // CELL_H
