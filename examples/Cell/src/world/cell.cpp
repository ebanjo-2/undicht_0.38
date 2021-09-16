#include <world/cell.h>

namespace cell {


    const unsigned char YP = 0x01; // + y (00000001)
    const unsigned char YN = 0x02; // - y (00000010)
    const unsigned char XP = 0x04; // + x (00000100)
    const unsigned char XN = 0x08; // - x (00001000)
    const unsigned char ZP = 0x10; // + z (00010000)
    const unsigned char ZN = 0x20; // - z (00100000)

    const unsigned char FACE_MASK[] = {YP, YN, XP, XN, ZP, ZN};
    const unsigned char OPPOSING_FACE_MASK[] = {YN, YP, XN, XP, ZN, ZP};

    const extern unsigned short VOID_CELL = -1;


    std::ostream& operator << (std::ostream& out, const Cell& c) {

        out << (int)c.m_pos0.x << " " << (int)c.m_pos0.y << " " << (int)c.m_pos0.z
        << " : " << (int)c.m_pos1.x << " " << (int)c.m_pos1.y << " " << (int)c.m_pos1.z;

        return out;
    }

} // cell
