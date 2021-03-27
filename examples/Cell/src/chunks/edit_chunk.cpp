#include "edit_chunk.h"
#include <iostream>

#include <math/math_tools.h>

namespace cell {

    using namespace undicht::tools;


    EditChunk::EditChunk() {

    }

    EditChunk::~EditChunk() {

    }




    //////////////////////////////////////////// internal functions to modify cells ////////////////////////////////////////////

	void EditChunk::getCellsSharingVolume(const Cell& c, std::vector<Cell*>& loadTo, std::vector<Cell>& shared_volume) {
		/** picks one of the search functions based on cs volume and the total number of cells */

		if (c.getVolume() > m_chunk_source->m_cells.size()) {

			getCellsSharingVolumeBySearchingAll(c, loadTo, shared_volume);
		} else {

			getCellsSharingVolumeBySearchingC(c, loadTo, shared_volume);
		}

	}


    void EditChunk::getCellsSharingVolumeBySearchingC(const Cell& c, std::vector<Cell*>& loadTo, std::vector<Cell>& shared_volume) {
        /** finds every cell of the current DrawChunk that shares some volume with c
        * @param shared_volume : the volume shared between each cell and c */

		glm::uvec3 point1 = c.getPoint1();
		glm::uvec3 point2 = c.getPoint2();

		// the pos that marches through the cell
		glm::uvec3 current_pos = point1;

		bool new_cell;

		do {
			
			// getting a cell that shares volume with c
			Cell* candidate = getCellAtPosition(current_pos);
			new_cell = true;

			// testing if the candidate was found before
			for (Cell* old_candidate : loadTo) {

				if(candidate == old_candidate) {
					new_cell = false; // cell was already found
					break;
				}
			}

			// if not, adding the new cell to the list of Cells sharing volume with c
			if (new_cell) {
				loadTo.push_back(candidate);
				shared_volume.push_back(candidate->getSharedVolume(c));
			}

			// dont need to check here anymore
			current_pos.x = std::max(current_pos.x, candidate->getPoint2().x - 1);

			// going to the next position
		} while (Cell::marchInCell(current_pos, point1, point2));

    }


	void EditChunk::getCellsSharingVolumeBySearchingAll(const Cell& c, std::vector<Cell*>& loadTo, std::vector<Cell>& shared_volume) {
		/** also finds every cell of the current DrawChunk that shares some volume with c
		* but does it not by checking every reference within the volume of c, but by
		* checking every Cell in m_chunk source (which may be less then cs volume) */

		for (Cell& existing_cell : m_chunk_source->m_cells) {

			if (existing_cell.sharesVolume(c)) {

				loadTo.push_back(&existing_cell);
				shared_volume.push_back(existing_cell.getSharedVolume(c));
			}

		}

	}

    ///////////////////////////////////// functions to modify DrawChunks ///////////////////////////////////



    void EditChunk::setCell(const Cell& c, DrawChunk* chunk) {
        /** sets everything inside the cell to be the material of the cell
        * will make sure that the DrawChunk gets updated correctly
        * but maybe not in the most efficient way */

        setChunkSource(chunk);

        std::vector<Cell*> to_update; // the cells that need to be updated
        std::vector<Cell> updates; // the updates to those cells

        getCellsSharingVolume(c, to_update, updates);

		std::vector<Cell> new_cells;

		// updating the cells
        for(int i = 0; i < to_update.size(); i++) {

            to_update[i]->setInCell(updates[i], new_cells);
        }

		// updating the references to the new cells
		for (int i = 0; i < new_cells.size(); i++) {

			chunk->m_cells.push_back(new_cells[i]);
			updateCellReference(new_cells[i], chunk->m_cells.size() - 1);
		}


    }




    unsigned short EditChunk::getMaterial(const glm::uvec3& pos, DrawChunk* chunk) {

		setChunkSource(chunk);

        return getCellAtPosition(pos)->mat;
    }

    ////////////////////////////// functions to reduce the size of DrawChunks //////////////////////////////


    void EditChunk::optDrawChunkByMerge(DrawChunk* c) {
        /** tries to minimize the size of the DrawChunk
        * by finding cells that can be merged */

    }


    void EditChunk::optDrawChunkComplete(DrawChunk* c) {
        /** tries to minimize the size of the DrawChunk
        * probably slower but more effective then optDrawChunkByMerge */

    }

} // cell
