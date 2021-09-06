#include <world/cell.h>

namespace cell {

    Cell::Cell() {

    }


    Cell::Cell(const u8vec3& pos0, const u8vec3& pos1, unsigned short mat) {

        m_pos0 = pos0;
        m_pos1 = pos1;
        m_material = mat;

    }

    void Cell::setMaterial(unsigned short mat) {

        m_material = mat;
    }


    unsigned short Cell::getMaterial() {

        return m_material;
    }

    std::ostream& operator << (std::ostream& out, const Cell& c) {

        out << (int)c.m_pos0.x << " " << (int)c.m_pos0.y << " " << (int)c.m_pos0.z
        << " : " << (int)c.m_pos1.x << " " << (int)c.m_pos1.y << " " << (int)c.m_pos1.z;

        return out;
    }

} // cell
