#include <geometry/geometry.h>
#include <iostream>

namespace undicht {

    namespace tools {

        bool Geometry::s_build_uvs = false;
        bool Geometry::s_build_normals = false;

        //////////////////////////// settings for geomtry generation ///////////////////////

        void Geometry::buildUVs(bool build_uvs) {

            s_build_uvs = build_uvs;
        }

        void Geometry::buildNormals(bool build_normals) {

            s_build_normals = build_normals;
        }


        int Geometry::getCurrVertexSize() {
            /** the size a vertex generated with the current settings would have (in bytes)
            * (position: 3 floats)
            * (uv: 2 floats)
            * (normal: 3 floats)*/

            int u = sizeof(float);

            return 3 * u + s_build_uvs * 2 * u + s_build_normals * 3 * u;
        }

        /////////////////////////////////// generating geometry //////////////////////////////

        void Geometry::genCuboid(glm::vec3 pos0, glm::vec3 pos1, std::vector<float>& vertices) {
            /** @param default orientation: pos0 +x +y +z, pos1 -x -y -z */

            std::vector<glm::vec3> pos = { // vertex positions of a cube
                glm::vec3(pos0.x, pos0.y, pos0.z),
                glm::vec3(pos0.x, pos0.y, pos1.z),
                glm::vec3(pos1.x, pos0.y, pos1.z),
                glm::vec3(pos1.x, pos0.y, pos0.z),

                glm::vec3(pos0.x, pos1.y, pos0.z),
                glm::vec3(pos0.x, pos1.y, pos1.z),
                glm::vec3(pos1.x, pos1.y, pos1.z),
                glm::vec3(pos1.x, pos1.y, pos0.z)
            };

            // all faces should be defined counter clockwise
            std::vector<int> ind = {
                0,1,2,3, // +y
                7,6,5,4, // -y
                1,0,4,5, // +x
                3,2,6,7, // -x
                0,3,7,4, // +z
                2,1,5,6  // -z
            };

            for(int i = 0; i < 6; i++) {
                // generating the faces

                std::vector<float> temp_vertices;
                std::vector<int> temp_indices;

                genRectangle(pos[ind[i*4 + 0]], pos[ind[i*4 + 1]], pos[ind[i*4 + 2]], pos[ind[i*4 + 3]], temp_vertices, temp_indices);
                applyIndices(temp_vertices, temp_indices, getCurrVertexSize() / sizeof(float));

                vertices.insert(vertices.end(), temp_vertices.begin(), temp_vertices.end());
            }


        }


        void Geometry::genRectangle(glm::vec2 pos0, glm::vec2 pos1, std::vector<float>& vertices, std::vector<int>& indices) {
            /** @param default orientation: pos0 upper left, pos1 lower right */

            if(!s_build_uvs) {

                vertices = {
                    pos0.x, pos0.y,
                    pos1.x, pos0.y,
                    pos0.x, pos1.y,
                    pos1.x, pos1.y
                };

            }

            if(s_build_uvs) {

                vertices = {
                    pos0.x, pos0.y, 0, 1,
                    pos1.x, pos0.y, 1, 1,
                    pos0.x, pos1.y, 0, 0,
                    pos1.x, pos1.y, 1, 0
                };

            }

            indices = {0,1,2, 1,3,2};
        }

        void Geometry::genRectangle(glm::vec3 pos0, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, std::vector<float>& vertices, std::vector<int>& indices) {
                /** @param default orientation: the positions are positioned clockwise on the plane
                * unless the two points share the same y coordinate, the rectangle is going to be vertical */

            if(!s_build_uvs) {

                vertices = {
                    pos0.x, pos0.y, pos0.z,
                    pos1.x, pos1.y, pos1.z,
                    pos2.x, pos2.y, pos2.z,
                    pos3.x, pos3.y, pos3.z
                };

            }

            if(s_build_uvs) {

                vertices = {
                    pos0.x, pos0.y, pos0.z, 0, 1,
                    pos1.x, pos1.y, pos1.z, 1, 1,
                    pos2.x, pos2.y, pos2.z, 1, 0,
                    pos3.x, pos3.y, pos3.z, 0, 0
                };

            }

            indices = {0,1,2, 0,2,3};

        }


        //////////////////////////////////////// manipulating existing geometry ////////////////////////////////////////


        void Geometry::applyIndices(std::vector<float>& vertices, const std::vector<int>& indices, int vertex_size) {
            /** @param vertex_size: number of floats, not bytes */

            int old_vertices_size = vertices.size();

            for(const int& i : indices) {

                vertices.insert(vertices.end(), vertices.begin() + i * vertex_size, vertices.begin() + (i + 1) * vertex_size);

            }

            vertices.erase(vertices.begin(), vertices.begin() + old_vertices_size);

        }


    } // tools

} // undicht
