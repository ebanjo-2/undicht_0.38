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

        void Geometry::genRectangle(glm::vec2 pos0, glm::vec2 pos1, std::vector<float>& vertices, std::vector<int>& indices) {
            /** @param default orientation: pos0 upper left, pos1 lower right */

            vertices = {
                pos0.x, pos0.y, 0, 1,
                pos1.x, pos0.y, 1, 1,
                pos0.x, pos1.y, 0, 0,
                pos1.x, pos1.y, 1, 0
            };

            indices = {0,1,2, 1,3,2};
        }

    } // tools

} // undicht
