#ifndef EDIT_CHUNK_H
#define EDIT_CHUNK_H

#include "draw_chunk.h"
#include "fast_access_chunk.h"
#include <vector>

namespace cell {

    class EditChunk : public FastAccessChunk {
        /** @brief this class allows to edit a DrawChunk */

    protected:
        // internal functions to modify cells

		/** picks one of the search functions based on cs volume and the total number of cells */
		void getCellsSharingVolume(const Cell& c, std::vector<int>& loadTo, std::vector<Cell>& shared_volume);


        /** finds every cell of the current DrawChunk that shares some volume with c
        * @param shared_volume : the volume shared between each cell and c
        * the material of the shared_volume cells will be the one provided with c */
        void getCellsSharingVolumeBySearchingC(const Cell& c, std::vector<int>& loadTo, std::vector<Cell>& shared_volume);

		/** also finds every cell of the current DrawChunk that shares some volume with c 
		* but does it not by checking every reference within the volume of c, but by 
		* checking every Cell in m_chunk source (which may be less then cs volume) */
		void getCellsSharingVolumeBySearchingAll(const Cell& c, std::vector<int>& loadTo, std::vector<Cell>& shared_volume);


    public:
        // functions to modify DrawChunks

        /** sets everything inside the cell to be the material of the cell
        * will make sure that the DrawChunk gets updated correctly
        * but maybe not in the most efficient way */
        void setCell(const Cell& c, DrawChunk* chunk);

        unsigned short getMaterial(const glm::uvec3& pos, DrawChunk* chunk);

    public:
        // functions to reduce the size of DrawChunks
        // Warning: very large cells are slower to edit

		/** tests if there are void cells around the drawn cells
		* if not, they dont need to be drawn
		* should be called after many cells were edited */
		void findHiddenCells(DrawChunk* c);

        /** tries to minimize the size of the DrawChunk
        * by finding cells that can be merged */
        void optDrawChunkByMerge(DrawChunk* c);

        /** tries to minimize the size of the DrawChunk
        * probably slower but more effective then optDrawChunkByMerge */
        void optDrawChunkComplete(DrawChunk* c);


    public:

        EditChunk();
        virtual ~EditChunk();

    };


} // cell


#endif // EDIT_CHUNK_H
