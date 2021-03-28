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

    Cell::Cell(const glm::uvec3& pos1, const glm::uvec3& pos2, unsigned short mat) {

		typedef unsigned char uc; // shut up, compiler

		this->pos1 = { (uc)pos1.x, (uc)pos1.y, (uc)pos1.z};
		this->pos2 = { uc(255 - pos2.x), uc(255 - pos2.y), uc(255 - pos2.z) };
		this->mat = mat;
    }



    Cell::~Cell() {

    }

    ////////////////////////////////////// setting the data of the cell //////////////////////////////////////

    void Cell::setPosition(const glm::uvec3& pos) {
        /** sets the position of the corner with the smallest coordinates */

        // keeping the texture orientation
		glm::bvec3 mirror = getOrientation();

        // keeping the cells size
		glm::uvec3 siz = getSize();

        for(int i = 0; i < 3; i++) {

            if(mirror[i]) { // flipped texture

                pos2[i] = 255 - pos[i];
            } else { // default

                pos1[i] = pos[i];
            }

        }

        setSize(siz);
    }

    void Cell::setSize(const glm::uvec3& siz) {

        // keeping the texture orientation
		const glm::bvec3& mirror = getOrientation();

        for(int i = 0; i < 3; i++) {

            if(mirror[i]) { // flipped texture

                pos1[i] = (255 - pos2[i]) + siz[i];
            } else { // default

                pos2[i] = 255 - (pos1[i] + siz[i]);
            }

        }

    }

    void Cell::setOrientation(const glm::bvec3& mirror) {
        /** sets whether the textures are mirrored in the directions */

		glm::uvec3 point1 = getPoint1();
		glm::uvec3 point2 = getPoint2();

        if(mirror.x) {
            pos1[0] = point2[0];
            pos2[0] = 255 - point1[0];
        } else { // default orientation
            pos1[0] = point1[0];
            pos2[0] = 255 - point2[0];
        }

        if(mirror.y) {
            pos1[1] = point2[1];
            pos2[1] = 255 - point1[1];
        } else { // default orientation
            pos1[1] = point1[1];
            pos2[1] = 255 - point2[1];
        }

        if(mirror.z) {
            pos1[2] = point2[2];
            pos2[2] = 255 - point1[2];
        } else { // default orientation
            pos1[2] = point1[2];
            pos2[2] = 255 - point2[2];
        }

    }


    ////////////////////////////////////// getting the data of the cell //////////////////////////////////////

    glm::uvec3 Cell::getPoint1() const {
        /** @return the point within the cell volume with the smallest x,y and z coords */

		glm::uvec3 p1;

        p1[0] = std::min<unsigned char>(pos1[0], 255 - pos2[0]);
        p1[1] = std::min<unsigned char>(pos1[1], 255 - pos2[1]);
        p1[2] = std::min<unsigned char>(pos1[2], 255 - pos2[2]);

        return p1;
    }

	glm::uvec3 Cell::getPoint2() const {
        /** @return the point within the cell volume with the biggest x,y and z coords */

		glm::uvec3 p2;

        p2[0] = std::max<unsigned char>(pos1[0], 255 - pos2[0]);
        p2[1] = std::max<unsigned char>(pos1[1], 255 - pos2[1]);
        p2[2] = std::max<unsigned char>(pos1[2], 255 - pos2[2]);

        return p2;
    }

	glm::uvec3 Cell::getSize() const {
        /** @return the number of ACells in the positive direction from getPoint1()
        * negative sizes still mean cells in the positive direction,
        * but the textures should be mirrored in that direction */

        // if pos1 is bigger than 255 - pos2, then this should automatically produce negative sizes
        short sx = short(255) - pos1[0] - pos2[0];
        short sy = short(255) - pos1[1] - pos2[1];
        short sz = short(255) - pos1[2] - pos2[2];

        return glm::uvec3(std::abs(sx), std::abs(sy), std::abs(sz));
    }


	glm::bvec3 Cell::getOrientation() {
        /** whether the textures are mirrored in the directions */

		glm::bvec3 mirror;

		mirror[0] = pos1[0] > (255 - pos2[0]); // if true, the texture should be mirrored
		mirror[1] = pos1[1] > (255 - pos2[1]);
		mirror[2] = pos1[2] > (255 - pos2[2]);

        return mirror;
    }

    unsigned int Cell::getVolume() const {

        glm::uvec3 s = getSize();

        return s.x * s.y * s.z;
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

		glm::uvec3 point0 = getPoint1();
		glm::uvec3 point1 = c.getPoint1();
		glm::uvec3 point2 = c.getPoint2();
		glm::uvec3 point3 = getPoint2();

		for (int i = 0; i < 3; i++) {

			n.pos1[i] = std::max(point0[i], point1[i]);
			n.pos2[i] = 255 - std::min(point2[i], point3[i]);
		}

        return n;
    }



    void Cell::setInCell(const Cell& c, std::vector<Cell>& new_cells) {
        /** splits this cell into new cells, covering the difference between c and this cell
        * at the end, this cell will be equal to c
        * @param c should be a part of this cells volume */

        // frequently used values
		glm::uvec3 point0 = getPoint1();
		glm::uvec3 point1 = c.getPoint1();
		glm::uvec3 point2 = c.getPoint2();
		glm::uvec3 point3 = getPoint2();

		if (point0.x != point1.x) {
			// new cell to the left
			new_cells.emplace_back(Cell(glm::uvec3(point0.x, point0.y, point0.z), glm::uvec3(point1.x, point3.y, point3.z), mat));
		}

		if (point0.y != point1.y) {
			// new cell below
			new_cells.emplace_back(Cell(glm::uvec3(point1.x, point0.y, point0.z), glm::uvec3(point2.x, point1.y, point3.z), mat));
		}

		if (point0.z != point1.z) {
			// new cell behind
			new_cells.emplace_back(Cell(glm::uvec3(point1.x, point1.y, point0.z), glm::uvec3(point2.x, point2.y, point1.z), mat));
		}

		if (point2.x != point3.x) {
			// new cell to the right
			new_cells.emplace_back(Cell(glm::uvec3(point2.x, point0.y, point0.z), glm::uvec3(point3.x, point3.y, point3.z), mat));
		}

		if (point2.y != point3.y) {
			// new cell above
			new_cells.emplace_back(Cell(glm::uvec3(point1.x, point2.y, point0.z), glm::uvec3(point2.x, point3.y, point3.z), mat));
		}

		if (point2.z != point3.z) {
			// new cell in front
			new_cells.emplace_back(Cell(glm::uvec3(point1.x, point1.y, point2.z), glm::uvec3(point2.x, point2.y, point3.z), mat));
		}

        // this cell now takes up the volume of c
        // with the rest of the old this being filled up with the new cells
		pos1 = c.pos1;
		pos2 = c.pos2;
		mat = c.mat;

    }

	/////////////////////////////////////////////// other useful functions ///////////////////////////////////////////////

	bool Cell::marchInCell(glm::uvec3& vec, const glm::uvec3& point1, const glm::uvec3& point2) {
		/** marches the vector through every position within the cell */

		vec.x++;

		if (vec.x >= point2.x) {
			vec.x = point1.x;
			vec.y++;
		}

		if (vec.y >= point2.y) {
			vec.y = point1.y;
			vec.z++;
		}

		if (vec.z >= point2.z) {
			return false;
		}

		return true;
	}
} // cell
