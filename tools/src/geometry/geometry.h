#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>

#include <vector>


namespace undicht {

    namespace tools {


        class Geometry {
                // a class that generates simple geometry

                static bool s_build_uvs;
                static bool s_build_normals;

            public:
                // settings for geomtry generation

                static void buildUVs(bool build_uvs);
                static void buildNormals(bool build_normals);

                /** the size a vertex generated with the current settings would have (in bytes)
                * (position: 3 floats)
                * (uv: 2 floats)
                * (normal: 3 floats)*/
                static int getCurrVertexSize();

            public:
                // generating geometry

                /** @param default orientation: pos0 +x +y +z, pos1 -x -y -z
                * faces will be in this order: +y -y +x -x +z -z*/
                static void genCuboid(glm::vec3 pos0, glm::vec3 pos1, std::vector<float>& vertices);

                /** @param default orientation: pos0 upper left, pos1 lower right */
                static void genRectangle(glm::vec2 pos0, glm::vec2 pos1, std::vector<float>& vertices, std::vector<int>& indices);

                /** @param default orientation: the positions are positioned clockwise on the plane
                * unless the two points share the same y coordinate, the rectangle is going to be vertical */
                static void genRectangle(glm::vec3 pos0, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, std::vector<float>& vertices, std::vector<int>& indices);


            public:
                // manipulating existing geometry

                /** @param vertex_size: number of floats, not bytes */
                static void applyIndices(std::vector<float>& vertices, const std::vector<int>& indices, int vertex_size);

        };


    } // tools

} // undicht


#endif
