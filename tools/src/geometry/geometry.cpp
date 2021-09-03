#include <geometry/geometry.h>


namespace undicht {

    namespace tools {

        bool Geometry::s_use_indices = true;
        bool Geometry::s_build_uvs = false;
        bool Geometry::s_build_normals = false;

        //////////////////////////// settings for geomtry generation ///////////////////////

        void Geometry::useIndices(bool use_indices) {

            s_use_indices = use_indices;
        }

        void Geometry::buildUVs(bool build_uvs) {

            s_build_uvs = build_uvs;
        }

        void Geometry::buildNormals(bool build_normals) {

            s_build_normals = build_normals;
        }

        /////////////////////////////////// generating geometry //////////////////////////////

        void Geometry::genCuboid(glm::vec3 pos0, glm::vec3 pos1, std::vector<float>& vertices, std::vector<int>& indices) {
            /** @param default orientation: pos0 +x +y +z, pos1 -x -y -z */

            vertices = {
                pos0.x, pos0.y, pos0.z,
                pos0.x, pos0.y, pos1.z,
                pos1.x, pos0.y, pos1.z,
                pos1.x, pos0.y, pos0.z,

                pos0.x, pos1.y, pos0.z,
                pos0.x, pos1.y, pos1.z,
                pos1.x, pos1.y, pos1.z,
                pos1.x, pos1.y, pos0.z
            };

            // all faces should be defined clockwise
            indices = {
                0,1,3, 1,2,3, // +y
                4,7,5, 5,7,6, // -y
                3,2,6, 3,6,7, // +x
                1,0,4, 5,1,4, // -x
                0,3,4, 3,7,4, // +z
                2,1,5, 2,5,6  // -z
            };


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


        void Geometry::applyIndices(const std::vector<float>& vertices, const std::vector<int>& indices, int vertex_size, std::vector<float>& loadTo_vertices) {

            for(const int& i : indices) {

                loadTo_vertices.insert(loadTo_vertices.end(), vertices.begin() + i * vertex_size, vertices.begin() + (i + 1) * vertex_size);

            }

        }


    } // tools

} // undicht
