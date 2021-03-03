#ifndef EDIT_CHUNK_H
#define EDIT_CHUNK_H

#include "draw_chunk.h"
#include <vector>

namespace cell {

    class EditChunk {
        /** @brief this class allows edit to a DrawChunk
        * the class stores a reference to the most recently edited draw chunk
        * because it can take a while to load a draw chunk into an edit chunk
        * so, unless the draw chunk is not going to be edited again, try avoiding
        * using the same EditChunk for multiple DrawChunks */

    public:

        // the most recently loaded DrawChunk
        DrawChunk* m_draw_chunk;

        // stores the cell ids
        // cell id being a refernce to the m_draw_chunk->m_cells vector
        std::vector<std::vector<std::vector<unsigned int>>> m_refs;

        // stores a bool for every cell
        std::vector<std::vector<std::vector<bool>>> m_stencil;

    protected:
        // internal functions

        // clears the buffers ( + if necessary, also sets the size to 256 * 256 * 256)
        void clearStencil();
        void clearRefs();

        // initializes the m_refs buffer to the DrawBuffer
        // only will do anything if the DrawBuffer is different to m_draw_chunk
        void loadDrawChunk(DrawChunk* c);

        // updates the refs inside the cells volume to ref_id
        void updateRefs(const Cell& c, int ref_id);

    protected:
        // internal functions to modify cells

        /** finds every cell of the current DrawChunk that shares some volume with c
        * @param shared_volume : the volume shared between each cell and c
        * the material of the shared_volume cells will be the one provided with c */
        void getCellsSharingVolume(const Cell& c, std::vector<Cell*>& loadTo, std::vector<Cell>& shared_volume);

        /** @return a cell representing the overlapping volume between the two cells
        * the material of the returned cell will be equal to c2s material
        * the returned cell will only be correct if they actually share volume */
        Cell getSharedVolume(const Cell* c1, const Cell* c2);

        /** @return true, if the cells volumes intersect at some point */
        bool cellsSharingVolume(const Cell* c1, const Cell* c2);

        /** @brief calculates the changes needed to set "edit" into the old cell
        * if old and edit share the same material, nothing happens
        * if old and edit share the same volume, olds material will be set to edits
        * if edit is part of old,
        *   + new cells will be created to take up the volume difference between old and edit (they will have old material)
        *   + the number of new cells will be returned
        *   + old will be replaced with edit */
        unsigned int editCell(Cell* old, Cell* edit, std::vector<Cell>& new_cells);

    public:
        // functions to modify DrawChunks

        /** sets everything inside the cell to be the material of the cell
        * will make sure that the DrawChunk gets updated correctly
        * but maybe not in the most efficient way */
        void setCell(const Cell& c, DrawChunk* chunk);

        /** @return the cell at the position
        * you may even change its material (nothing else though)
        * dont expect any drawn changes before updateDrawBuffer was called though */
        Cell& getCell(unsigned char x, unsigned char y, unsigned char z, DrawChunk* chunk);

        unsigned short getMaterial(unsigned char x, unsigned char y, unsigned char z, DrawChunk* chunk);

    public:
        // functions to reduce the size of DrawChunks
        // Warning: very large cells are slower to edit

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
