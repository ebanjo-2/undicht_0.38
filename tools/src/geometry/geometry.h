#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>

#include <vector>


namespace undicht {

    namespace tools {


        class Geometry {
                // a class that generates simple geometry

                static bool s_use_indices;
                static bool s_build_uvs;
                static bool s_build_normals;

            public:
                // settings for geomtry generation

                static void useIndices(bool use_indices);
                static void buildUVs(bool build_uvs);
                static void buildNormals(bool build_normals);

            public:
                // generating geometry

                /** @param default orientation: pos0 +x +y +z, pos1 -x -y -z */
                static void genCuboid(glm::vec3 pos0, glm::vec3 pos1, std::vector<float>& vertices, std::vector<int>& indices);

                /** @param default orientation: pos0 upper left, pos1 lower right */
                static void genRectangle(glm::vec2 pos0, glm::vec2 pos1, std::vector<float>& vertices, std::vector<int>& indices);

        };


    } // tools

} // undicht


#endif
