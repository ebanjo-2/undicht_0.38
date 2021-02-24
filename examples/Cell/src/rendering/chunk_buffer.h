#ifndef CHUNK_BUFFER_H
#define CHUNK_BUFFER_H

#include <core/vertex_buffer.h>
#include <chunk.h>

using namespace undicht;
using namespace tools;
using namespace graphics;

/** generates the vertices of a cube and stores them in the vertex buffer */
void loadCubeVertices(VertexBuffer& loadTo);

template <unsigned int CHUNK_SIZE>
class ChunkBuffer : public VertexBuffer {
        /** stores the cells of a compact chunk
        * so that they can be accessed by the gpu */
    public:

        void setCellData(const CompactChunk<CHUNK_SIZE>& chunk_data) {

            std::vector<int> cell_data;

            for (const CellChunk<CHUNK_SIZE>& cc : chunk_data.m_cells) {

                cell_data.push_back(cc.m_pos_x);
                cell_data.push_back(cc.m_pos_y);
                cell_data.push_back(cc.m_pos_z);

                cell_data.push_back(cc.m_size_x + 1);
                cell_data.push_back(cc.m_size_y + 1);
                cell_data.push_back(cc.m_size_z + 1);

                cell_data.push_back(cc.m_material);
            }

            // each instance of the cube drawn has got its own position, size and material
            setInstanceData(cell_data.data(), cell_data.size() * sizeof(int));

        }

        ChunkBuffer() {

            loadCubeVertices(*this);

            BufferLayout instance_layout({ UND_VEC3I, UND_VEC3I, UND_INT });
            setInstanceLayout(instance_layout);

        }


        virtual ~ChunkBuffer() = default;
};

#endif // CHUNK_BUFFER_H
