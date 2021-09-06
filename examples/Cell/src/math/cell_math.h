#ifndef CELL_MATH_H
#define CELL_MATH_H

#include <glm/glm.hpp>
#include <array>

#include <world/cell.h>

namespace cell {

    ///////////////////////////// basic 3d calculations ///////////////////////////////////

    /** volume of a cuboid */
    int getVolume(const Cell& c);

    /////////////////////////// relations between cells ///////////////////////////////////

    /** @return true, if the cells share some volume (touching isnt enough) */
    bool overlappingVolume(const Cell& c0, const Cell& c1);

    /** @return the volume shared by both cells
    * only works if the actually share some volume (check with overlappingVolume) */
    Cell getSharedVolume(const Cell& c0, const Cell& c1);

    /** c1 should be part of c0
    * creates 6 cells covering the volume difference between c0 and c1
    * (some of those cells may have a volume of 0) */
    void subtractCells(const Cell& c0, const Cell& c1, std::array<Cell, 6> &diff);

} // cell

#endif // CELL_MATH_H