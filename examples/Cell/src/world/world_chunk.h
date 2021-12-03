#ifndef WORLD_CHUNK_H
#define WORLD_CHUNK_H


#include <world/chunk.h>

namespace cell {

    class WorldChunk : public Chunk {
            // contains information about a chunk placed into the world
            // such as its offset and changes made to it

        protected:

            // the world position of the 0,0,0 block of the chunk
            glm::ivec3 m_origin;

            int m_last_optimized_cell_count = 0;

            /** contains data about every cell in the chunk
            * 6 byte positions ( 2 * 3 bytes for u8vec)
            * 2 byte material id
            * 1 byte face mask (stores what face of the cell should get drawn */
            undicht::graphics::VertexBuffer m_vertex_buffer;

        public:

            WorldChunk();
            WorldChunk(const glm::ivec3& origin);

        public:
            // using the chunk at its position in the world

            /** @return true, if the pos is within the 255*255*255 chunk starting at getOrigin() */
            bool isInside(const glm::ivec3& pos); // thats what she said

            // no override
            /** the pos actually has to be within the chunk,
            * otherwise its not defined which cell gets returned */
            Cell getCellAt(const glm::ivec3& pos);


        public:
            // estimating the optimization need for the chunk

            /** @return a rough estimation of how much a optimization of the chunk is needed
            * 0 can mean that there is no need, the heigher the return,
            * the more cells were added to the chunk and the heigher the need for optimization */
            int getOptNeed() const;

            /** may be used when a chunk is loaded from a file */
            void setOptNeed(int need);

            /** tells the chunk that its just been optimized
            * getOptNeed() will now return 0 until changes are made to the chunk*/
            void markAsOptimzed();

        public:

            const glm::ivec3& getOrigin() const;
            void setOrigin(const glm::ivec3& origin);

        public:
        // adding / removing cells from m_cells
        // + updating the draw buffer afterwards

            virtual int addCell(const Cell& c);

            virtual void remCell(int id);

        public:
            // creating and maintaining the data to draw the chunk

            void initDrawBuffer();

            /** resizes the buffer that stores the data for every cell within the chunk
            * + fills the resized buffer with all the cells of the chunk */
            void resizeDrawBuffer(int cell_count);

            /** updates the data of the cell within the vertex_buffer
            * will also resize the buffer to fit new cells */
            void updateDrawBuffer(int id);

            /** updates the draw buffer for every cell of the chunk */
            void updateDrawBuffer();

            undicht::graphics::VertexBuffer& getDrawBuffer();

    };


} // cell

#endif // WORLD_CHUNK_H
