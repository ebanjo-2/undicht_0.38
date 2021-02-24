#include "chunk_buffer.h"


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

void loadCubeVertices(VertexBuffer& loadTo) {


    // building the vertices of a cube


    float positions[] = CUBE_POSITIONS;
    float uvs[] = CUBE_UVS;
    float normals[] = CUBE_NORMALS;

    int indices[] = CUBE_INDICES;
    int uv_indices[] = CUBE_UV_INDICES;

    // there are 6 faces, so 12 triangles with 3 vertices each and 8 floats per vertex
    std::array<float, 12 * 3 * 8> vertices;

    for (int i = 0; i < 12 * 3; i++) { // going through every vertex of the cube

        vertices[i * 8 + 0] = positions[indices[i] * 3 + 0];
        vertices[i * 8 + 1] = positions[indices[i] * 3 + 1];
        vertices[i * 8 + 2] = positions[indices[i] * 3 + 2];

        vertices[i * 8 + 3] = uvs[uv_indices[(i % 6)] * 2 + 0];
        vertices[i * 8 + 4] = uvs[uv_indices[(i % 6)] * 2 + 1];

        vertices[i * 8 + 5] = normals[(i / 6) * 3 + 0];
        vertices[i * 8 + 6] = normals[(i / 6) * 3 + 1];
        vertices[i * 8 + 7] = normals[(i / 6) * 3 + 2];
    }

    BufferLayout layout({ UND_VEC3F, UND_VEC2F, UND_VEC3F });

    loadTo.setLayout(layout);
    loadTo.setData(vertices.data(), sizeof(vertices));

}
