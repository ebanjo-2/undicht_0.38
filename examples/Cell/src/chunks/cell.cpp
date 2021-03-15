#include "cell.h"
#include <math/math_tools.h>

#include <iostream>

using namespace undicht;
using namespace tools;

namespace cell {

    Cell::Cell() {

    }

    Cell::Cell(unsigned short mat) {

        this->mat = mat;
    }

    Cell::Cell(std::array<unsigned char,3> pos, std::array<unsigned char,3> siz, unsigned short mat) {

        setPosition(pos);
        setSize(siz);
        this->mat = mat;
    }



    Cell::~Cell() {

    }

    ////////////////////////////////////// setting the data of the cell //////////////////////////////////////

    void Cell::setPosition(std::array<unsigned char,3> pos) {
        /** sets the position of the corner with the smallest coordinates */

        // keeping the texture orientation
        std::array<bool,3> orient = getOrientation();

        // keeping the cells size
        std::array<unsigned char,3> siz = getSize();

        for(int i = 0; i < 3; i++) {

            if(orient[i]) { // flipped texture

                pos2[i] = 255 - pos[i];
            } else { // default

                pos1[i] = pos[i];
            }

        }

        setSize(siz);
    }

    void Cell::setSize(std::array<unsigned char,3> siz) {

        // keeping the texture orientation
        std::array<bool,3> orient = getOrientation();

        for(int i = 0; i < 3; i++) {

            if(orient[i]) { // flipped texture

                pos1[i] = (255 - pos2[i]) + siz[i];
            } else { // default

                pos2[i] = 255 - (pos1[i] + siz[i]);
            }

        }

    }

    void Cell::setOrientation(bool rot_x, bool rot_y, bool rot_z) {
        /** sets whether the textures are mirrored in the directions */

        std::array<unsigned char,3> point1 = getPoint1();
        std::array<unsigned char,3> point2 = getPoint2();

        if(rot_x) {
            pos1[0] = point2[0];
            pos2[0] = 255 - point1[0];
        } else { // default orientation
            pos1[0] = point1[0];
            pos2[0] = 255 - point2[0];
        }

        if(rot_y) {
            pos1[1] = point2[1];
            pos2[1] = 255 - point1[1];
        } else { // default orientation
            pos1[1] = point1[1];
            pos2[1] = 255 - point2[1];
        }

        if(rot_z) {
            pos1[2] = point2[2];
            pos2[2] = 255 - point1[2];
        } else { // default orientation
            pos1[2] = point1[2];
            pos2[2] = 255 - point2[2];
        }

    }


    ////////////////////////////////////// getting the data of the cell //////////////////////////////////////

    std::array<unsigned char,3> Cell::getPoint1() const {
        /** @return the point within the cell volume with the smallest x,y and z coords */

        std::array<unsigned char,3> p1;

        p1[0] = std::min<unsigned char>(pos1[0], 255 - pos2[0]);
        p1[1] = std::min<unsigned char>(pos1[1], 255 - pos2[1]);
        p1[2] = std::min<unsigned char>(pos1[2], 255 - pos2[2]);

        return p1;
    }

    std::array<unsigned char,3> Cell::getPoint2() const {
        /** @return the point within the cell volume with the biggest x,y and z coords */

        std::array<unsigned char,3> p2;

        p2[0] = std::max<unsigned char>(pos1[0], 255 - pos2[0]);
        p2[1] = std::max<unsigned char>(pos1[1], 255 - pos2[1]);
        p2[2] = std::max<unsigned char>(pos1[2], 255 - pos2[2]);

        return p2;
    }

    std::array<unsigned char,3> Cell::getSize() const {
        /** @return the number of ACells in the positive direction from getPoint1()
        * negative sizes still mean cells in the positive direction,
        * but the textures should be mirrored in that direction */

        // if pos1 is bigger than 255 - pos2, then this should automatically produce negative sizes
        short sx = short(255) - pos1[0] - pos2[0];
        short sy = short(255) - pos1[1] - pos2[1];
        short sz = short(255) - pos1[2] - pos2[2];

        return {std::abs(sx), std::abs(sy), std::abs(sz)};
    }


    std::array<bool,3> Cell::getOrientation() {
        /** whether the textures are mirrored in the directions */

        std::array<bool,3> orient;

        orient[0] = pos1[0] > (255 - pos2[0]); // if true, the texture should be mirrored
        orient[1] = pos1[1] > (255 - pos2[1]);
        orient[2] = pos1[2] > (255 - pos2[2]);

        return orient;
    }

    unsigned int Cell::getVolume() const {

        std::array<unsigned char,3> s = getSize();

        return s[0] * s[1] * s[2];
    }

    ////////////////////////////////////// functions to modify the volume of the cell //////////////////////////////////////

    bool Cell::sharesVolume(const Cell& c) {
        /** @return true, if the volumes of the cells overlap */

        if(!overlappingRanges<int>(pos1[0], 255 - pos2[0], c.pos1[0], 255 - c.pos2[0])) return false;
        if(!overlappingRanges<int>(pos1[1], 255 - pos2[1], c.pos1[1], 255 - c.pos2[1])) return false;
        if(!overlappingRanges<int>(pos1[2], 255 - pos2[2], c.pos1[2], 255 - c.pos2[2])) return false;

        return true;
    }

    Cell Cell::getSharedVolume(const Cell& c) {
        /** @return a cell that covers the volume shared by this cell and c
        * the returned cells material is equal to cs material */

        Cell n(c.mat);

        for(int i = 0; i < 3; i++) {

            n.pos1[i] = std::max(getPoint1()[i], c.getPoint1()[i]);
            n.pos2[i] = 255 - std::min(getPoint2()[i], c.getPoint2()[i]);
        }

        return n;
    }



    void Cell::setInCell(const Cell& c, std::vector<Cell>& new_cells) {
        /** splits this cell into new cells, covering the difference between c and this cell
        * at the end, this cell will be equal to c
        * @param c should be a part of this cells volume */

        // frequently used values
        std::array<unsigned char,3> point1 = getPoint1();
        std::array<unsigned char,3> point2 = getPoint2();
        std::array<unsigned char,3> siz = getSize();

        std::array<unsigned char,3> cpoint1 = c.getPoint1();
        std::array<unsigned char,3> cpoint2 = c.getPoint2();
        std::array<unsigned char,3> csiz = c.getSize();


        // creating new cells surrounding c filling up the volume difference between this and c
        Cell bottom(mat);
        bottom.setPosition(point1);
        bottom.setSize({siz[0], cpoint1[1] - point1[1], siz[2]});

        Cell top(mat);
        top.setPosition({point1[0], cpoint2[1], point1[2]});
        top.setSize({siz[0], point2[1] - cpoint2[1], siz[2]});

        Cell left(mat);
        left.setPosition({point1[0], cpoint1[1], point1[2]});
        left.setSize({cpoint1[0] - point1[0], cpoint2[1] - cpoint1[1], siz[2]});

        Cell right(mat);
        right.setPosition({cpoint2[0], cpoint1[1], point1[2]});
        right.setSize({point2[0] - cpoint2[0], cpoint2[1] - cpoint1[1], siz[2]});

        Cell frnt(mat); // front
        frnt.setPosition({cpoint1[0], cpoint1[1], point1[2]});
        frnt.setSize({csiz[0], csiz[1], cpoint1[2] - point1[2]});

        Cell bck(mat); // back
        bck.setPosition({cpoint1[0], cpoint1[1], cpoint2[2]});
        bck.setSize({csiz[0], csiz[1], point2[2] - cpoint2[2]});

        // testing if the new cells are valid (or if c touches this cells border)
        if(point1[1] != cpoint1[1]) new_cells.push_back(bottom);
        if(point2[1] != cpoint2[1]) new_cells.push_back(top);
        if(point1[0] != cpoint1[0]) new_cells.push_back(left);
        if(point2[0] != cpoint2[0]) new_cells.push_back(right);
        if(point1[2] != cpoint1[2]) new_cells.push_back(frnt);
        if(point2[2] != cpoint2[2]) new_cells.push_back(bck);

        // this cell now takes up the volume of c
        // with the rest of the old this being filled up with the new cells
        setPosition(c.getPoint1());
        setSize(c.getSize());
        mat = c.mat;

    }


} // cell
