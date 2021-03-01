#include "draw_chunk.h"

namespace cell {

    std::array<float, 12 * 3 * 8> DrawChunk::s_cube_vertices;
    undicht::tools::BufferLayout DrawChunk::s_cube_layout;

    bool DrawChunk::s_cube_initialized = false;

    using namespace undicht;
    using namespace tools;
    using namespace graphics;


    DrawChunk::DrawChunk() {

        if(!s_cube_initialized) initCubeVertices();

        m_buffer.setLayout(s_cube_layout);
        m_buffer.setData(s_cube_vertices.data(), sizeof(s_cube_vertices));

    }

    DrawChunk::~DrawChunk() {


    }


    void DrawChunk::updateCellBuffer() {

        std::vector<int> cell_data;

        for (const Cell c : m_cells) {

            if(c.mat == 0) continue;

            cell_data.push_back(c.pos[0]);
            cell_data.push_back(c.pos[1]);
            cell_data.push_back(c.pos[2]);

            cell_data.push_back(c.siz[0] + 1); // the smallest size (0) should translate to a side length of 1
            cell_data.push_back(c.siz[1] + 1);
            cell_data.push_back(c.siz[2] + 1);

            cell_data.push_back(c.mat);
        }

        // each instance of the cube drawn has got its own position, size and material
        BufferLayout instance_layout({ UND_VEC3I, UND_VEC3I, UND_INT });
        m_buffer.setInstanceLayout(instance_layout);
        m_buffer.setInstanceData(cell_data.data(), cell_data.size() * sizeof(int));

    }

    ////////////////////////////////////////// building cube vertices //////////////////////////////////////////


    // cube mesh build with help from here:
    // https://pastebin.com/DXKEmvap

    // starting with the 4 vertices on the back
#define CUBE_POSITIONS {0,0,0, 1,0,0, 1,1,0, 0,1,0, 0,0,1, 1,0,1, 1,1,1, 0,1,1}
#define CUBE_UVS {0,0, 1,0, 1,1, 0,1}
#define CUBE_NORMALS {0,0,1, 1,0,0, 0,0,-1, -1,0,0, 0,1,0, 0,-1,0}

    //#define CUBE_INDICES {0, 1, 3, 3, 1, 2, 1, 5, 2, 2, 5, 6, 5, 4, 6, 6, 4, 7, 4, 0, 7, 7, 0, 3, 3, 2, 7, 7, 2, 6, 4, 5, 0, 0, 5, 1}
#define CUBE_INDICES {0, 3, 1, 3, 2, 1, 1, 2, 5, 2, 6, 5, 5, 6, 4, 6, 7, 4, 4, 7, 0, 7, 3, 0, 3, 7, 2, 7, 6, 2, 4, 0, 5, 0, 1, 5}
    //#define CUBE_UV_INDICES {0, 1, 3, 3, 1, 2}
#define CUBE_UV_INDICES {0, 3, 1, 3, 2, 1}

    void DrawChunk::initCubeVertices() {


        // building the vertices of a cube


        float positions[] = CUBE_POSITIONS;
        float uvs[] = CUBE_UVS;
        float normals[] = CUBE_NORMALS;

        int indices[] = CUBE_INDICES;
        int uv_indices[] = CUBE_UV_INDICES;


        for (int i = 0; i < 12 * 3; i++) { // going through every vertex of the cube

            s_cube_vertices[i * 8 + 0] = positions[indices[i] * 3 + 0];
            s_cube_vertices[i * 8 + 1] = positions[indices[i] * 3 + 1];
            s_cube_vertices[i * 8 + 2] = positions[indices[i] * 3 + 2];

            s_cube_vertices[i * 8 + 3] = uvs[uv_indices[(i % 6)] * 2 + 0];
            s_cube_vertices[i * 8 + 4] = uvs[uv_indices[(i % 6)] * 2 + 1];

            s_cube_vertices[i * 8 + 5] = normals[(i / 6) * 3 + 0];
            s_cube_vertices[i * 8 + 6] = normals[(i / 6) * 3 + 1];
            s_cube_vertices[i * 8 + 7] = normals[(i / 6) * 3 + 2];
        }

        s_cube_layout = BufferLayout({ UND_VEC3F, UND_VEC2F, UND_VEC3F });

        s_cube_initialized = true;
    }

} // cell
