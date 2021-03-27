#ifndef CELL_H
#define CELL_H

#include <array>
#include <vector>

#include <glm/glm.hpp>

namespace cell {

    class Cell {
        /** describes a volume of ACells
        * should be more efficient than storing every ACell */

    public:
        // members defining the volume
        // a cells dimensions are bound to the size of a chunk, so unsigned char is big enough

        std::array<unsigned char,3> pos1; // stores the distances of the cells borders to the 0 coordinate
        std::array<unsigned char,3> pos2; // stores the distances of the cells other borders to the 255 coordinate
        unsigned short mat;

    public:
        // setting the data of the cell

        /** sets the position of the corner with the smallest coordinates */
        void setPosition(const glm::uvec3& pos);

        void setSize(const glm::uvec3& siz);

        /** sets whether the textures are mirrored in the directions */
        void setOrientation(const glm::bvec3& mirror);


    public:
        // getting the data of the cell

        /** @return the point within the cell volume with the smallest x,y and z coords */
		glm::uvec3 getPoint1() const;

        /** @return the point within the cell volume with the biggest x,y and z coords */
		glm::uvec3 getPoint2() const;

        /** @return the number of ACells in the positive direction from getPoint1()
        * negative sizes still mean cells in the positive direction,
        * but the textures should be mirrored in that direction */
		glm::uvec3 getSize() const;

        /** whether the textures are mirrored in the directions */
		glm::bvec3 getOrientation();

        unsigned int getVolume() const;

    public:
        // functions to modify the volume of the cell

        /** @return true, if the volumes of the cells overlap */
        bool sharesVolume(const Cell& c);

        /** @return a cell that covers the volume shared by this cell and c
        * the returned cells material is equal to cs material */
        Cell getSharedVolume(const Cell& c);

        /** splits this cell into new cells, covering the difference between c and this cell
        * at the end, this cell will be equal to c
        * @param c should be a part of this cells volume */
        void setInCell(const Cell& c, std::vector<Cell>& new_cells);

	public:
		// other useful functions

		/** marches the vector through every position within the cell
		* @return false, if the end of the cell was reached */
		static bool marchInCell(glm::uvec3& vec, const glm::uvec3& point1, const glm::uvec3& point2);

    public:

        Cell();
        Cell(unsigned short mat);
        Cell(const glm::uvec3& pos1, const glm::uvec3& pos2, unsigned short mat);
        virtual ~Cell();

    };


} // cell

#endif // CELL_H
