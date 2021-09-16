#include <math/cell_math.h>

#include <math/math_tools.h>
#include <algorithm>


namespace cell {

    using namespace undicht;
    using namespace tools;

    ///////////////////////////// basic 3d calculations ///////////////////////////////////

    int getVolume(const Cell& c) {
        /** volume of a cuboid */

        int w = c.m_pos0.x - c.m_pos1.x;
        int h = c.m_pos0.y - c.m_pos1.y;
        int d = c.m_pos0.z - c.m_pos1.z;

        return glm::abs(w * h * d);
    }

    int getVolume(const u8vec3& pos0, const u8vec3& pos1) {

        int w = pos0.x - pos1.x;
        int h = pos0.y - pos1.y;
        int d = pos0.z - pos1.z;

        return glm::abs(w * h * d);
    }


    bool isPointInside(const Cell& c, const u8vec3& point) {

        if(!overlappingRanges(c.m_pos0.x, c.m_pos1.x, point.x, point.x))
            return false;
        if(!overlappingRanges(c.m_pos0.y, c.m_pos1.y, point.y, point.y))
            return false;
        if(!overlappingRanges(c.m_pos0.z, c.m_pos1.z, point.z, point.z))
            return false;

        return true;

    }


    /////////////////////////// relations between cells ///////////////////////////////////

    bool overlappingVolume(const Cell& c0, const Cell& c1) {
        /** @return true, if the cells share some volume (touching isnt enough) */

        if(!overlappingRanges(c0.m_pos0.x, c0.m_pos1.x, c1.m_pos0.x, c1.m_pos1.x))
            return false;
        if(!overlappingRanges(c0.m_pos0.y, c0.m_pos1.y, c1.m_pos0.y, c1.m_pos1.y))
            return false;
        if(!overlappingRanges(c0.m_pos0.z, c0.m_pos1.z, c1.m_pos0.z, c1.m_pos1.z))
            return false;

        return true;
    }

        /** @return true, if the cells share some volume (touching isnt enough) */
    bool overlappingVolume(const TCell<int>& c0, const TCell<int>& c1) {

        if(!overlappingRanges(c0.m_pos0.x, c0.m_pos1.x, c1.m_pos0.x, c1.m_pos1.x))
            return false;
        if(!overlappingRanges(c0.m_pos0.y, c0.m_pos1.y, c1.m_pos0.y, c1.m_pos1.y))
            return false;
        if(!overlappingRanges(c0.m_pos0.z, c0.m_pos1.z, c1.m_pos0.z, c1.m_pos1.z))
            return false;

        return true;
    }

    Cell getSharedVolume(const Cell& c0, const Cell& c1) {
        /** @return the volume shared by both cells
        * only works if the actually share some volume (check with overlappingVolume) */

        std::array<unsigned char,2> xcoords = getMidValues(c0.m_pos0.x, c0.m_pos1.x, c1.m_pos0.x, c1.m_pos1.x);
        std::array<unsigned char,2> ycoords = getMidValues(c0.m_pos0.y, c0.m_pos1.y, c1.m_pos0.y, c1.m_pos1.y);
        std::array<unsigned char,2> zcoords = getMidValues(c0.m_pos0.z, c0.m_pos1.z, c1.m_pos0.z, c1.m_pos1.z);

        Cell result;
        result.m_pos0 = u8vec3(xcoords[0], ycoords[0], zcoords[0]);
        result.m_pos1 = u8vec3(xcoords[1], ycoords[1], zcoords[1]);

        return result;
    }

    TCell<int> getSharedVolume(const TCell<int>& c0, const TCell<int>& c1) {

        std::array<int,2> xcoords = getMidValues(c0.m_pos0.x, c0.m_pos1.x, c1.m_pos0.x, c1.m_pos1.x);
        std::array<int,2> ycoords = getMidValues(c0.m_pos0.y, c0.m_pos1.y, c1.m_pos0.y, c1.m_pos1.y);
        std::array<int,2> zcoords = getMidValues(c0.m_pos0.z, c0.m_pos1.z, c1.m_pos0.z, c1.m_pos1.z);

        TCell<int> result;
        result.m_pos0 = glm::ivec3(xcoords[0], ycoords[0], zcoords[0]);
        result.m_pos1 = glm::ivec3(xcoords[1], ycoords[1], zcoords[1]);

        return result;
    }

    void subtractCells(const Cell& c0, const Cell& c1, std::array<Cell, 6> &diff) {
        /** c1 should be part of c0
        * creates 6 cells covering the volume difference between c0 and c1
        * (some of those cells may have a volume of 0)
        * order of the cells created (direction seen from c1) :
        * +y -y +x -x +z -z */

        std::array<unsigned char,4> xcoords = {c0.m_pos0.x, c0.m_pos1.x, c1.m_pos0.x, c1.m_pos1.x};
        std::array<unsigned char,4> ycoords = {c0.m_pos0.y, c0.m_pos1.y, c1.m_pos0.y, c1.m_pos1.y};
        std::array<unsigned char,4> zcoords = {c0.m_pos0.z, c0.m_pos1.z, c1.m_pos0.z, c1.m_pos1.z};

        // pos1 may not have greater values then pos0
        // which is why they have to be sorted
        std::sort(xcoords.begin(), xcoords.end());
        std::sort(ycoords.begin(), ycoords.end());
        std::sort(zcoords.begin(), zcoords.end());

        diff[0] = Cell(u8vec3(xcoords[0], ycoords[2], zcoords[0]), u8vec3(xcoords[3], ycoords[3], zcoords[3])); // top cell
        diff[1] = Cell(u8vec3(xcoords[0], ycoords[0], zcoords[0]), u8vec3(xcoords[3], ycoords[1], zcoords[3])); // bottom cell
        diff[2] = Cell(u8vec3(xcoords[2], ycoords[1], zcoords[0]), u8vec3(xcoords[3], ycoords[2], zcoords[3])); // right cell
        diff[3] = Cell(u8vec3(xcoords[0], ycoords[1], zcoords[0]), u8vec3(xcoords[1], ycoords[2], zcoords[3])); // left cell
        diff[4] = Cell(u8vec3(xcoords[1], ycoords[1], zcoords[2]), u8vec3(xcoords[2], ycoords[2], zcoords[3])); // front cell
        diff[5] = Cell(u8vec3(xcoords[1], ycoords[1], zcoords[0]), u8vec3(xcoords[2], ycoords[2], zcoords[1])); // back cell

    }

} // cell
