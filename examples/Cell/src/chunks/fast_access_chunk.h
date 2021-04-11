#include <chunks/draw_chunk.h>
#include <glm/glm.hpp>

namespace cell {

	class FastAccessChunk {
		/** used to access the cells of a DrawChunk at the given position */

	protected:
		// protected members and functions to keep track of the cells within the chunk_source

		std::vector<std::vector<std::vector<int>>> m_cell_references;

		DrawChunk* m_chunk_source = 0;

		/** sets all the cell references within c to id
		* used by EditChunk */
		void updateCellReference(const Cell& c, int id);

		/** resizes the m_cell_references vector to 256 * 256 * 256
		* + sets all references to 0 */
		void clearCellReferences();

	public:
		// functions for accessing cells

		/** sets the chunk of which the Cells are accessed */
		void setChunkSource(DrawChunk* c);

		const Cell* getCellAtPosition(const glm::uvec3& pos);

		int getCellIDAtPosition(const glm::uvec3& pos);


	public:

		FastAccessChunk();
		FastAccessChunk(DrawChunk* c);
		virtual ~FastAccessChunk();

	};


} // cell