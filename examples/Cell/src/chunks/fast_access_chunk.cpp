#include <chunks/fast_access_chunk.h>


namespace cell {

	FastAccessChunk::FastAccessChunk() {

		clearCellReferences();
	}


	FastAccessChunk::FastAccessChunk(DrawChunk* c) {

		clearCellReferences();
		setChunkSource(c);
	}

	FastAccessChunk::~FastAccessChunk() {

	}

	///////////// protected members and functions to keep track of the cells within the chunk_source ///////////////


	void FastAccessChunk::updateCellReference(const Cell& c, unsigned int id) {
		/** sets all the cell references within c to id
		* used by EditChunk */

		glm::uvec3 point1 = c.getPoint1();
		glm::uvec3 point2 = c.getPoint2();

		glm::uvec3 iter = point1;

		do {

			m_cell_references[iter.x][iter.y][iter.z] = id;

		} while (Cell::marchInCell(iter, point1, point2));


	}


	void FastAccessChunk::clearCellReferences() {
		/** resizes the m_cell_references vector to 256 * 256 * 256
		* + sets all references to 0 */

		m_cell_references.resize(255);
		for (auto& v : m_cell_references) {
			v.resize(255);
			for (auto& vv : v) vv.resize(255);
		}

	}

	//////////////////////////////////////// functions for accessing cells ////////////////////////////////////////

	void FastAccessChunk::setChunkSource(DrawChunk* c) {
		/** sets the chunk of which the Cells are accessed */

		if (m_chunk_source == c) return;

		m_chunk_source = c;

		for (int i = 0; i < c->m_cells.size(); i++) {

			updateCellReference(c->m_cells[i], i);
		}

	}

	Cell* FastAccessChunk::getCellAtPosition(const glm::uvec3& pos) {

		if (!m_chunk_source) {

			return 0;
		}

		return &m_chunk_source->m_cells[m_cell_references[pos.x][pos.y][pos.z]];
	}

}