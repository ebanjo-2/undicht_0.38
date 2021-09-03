#ifndef CELL_RENDERER_H
#define CELL_RENDERER_H

#include <core/renderer.h>
#include <camera/perspective_camera_3d.h>

#include <world/cell.h>
#include <world/chunk.h>

#include <rendering/texture_atlas.h>

namespace cell {

    class CellRenderer : public undicht::graphics::Renderer {

            undicht::graphics::Shader m_shader;
            undicht::graphics::VertexBuffer m_vertex_buffer;

            undicht::graphics::Uniform m_view;
            undicht::graphics::Uniform m_proj;
            undicht::graphics::Uniform m_chunk_pos;
            undicht::graphics::Uniform m_pos0;
            undicht::graphics::Uniform m_pos1;


        public:

            CellRenderer();
            virtual ~CellRenderer();

            void init();
            void term();

            void loadCam(undicht::graphics::PerspectiveCamera3D& player_cam);

            void loadTextureAtlas(const TextureAtlas& atlas);

            /** draws a single cell at the cell_pos relative to 0 | 0 | 0
            * not bound to chunks */
            void draw(const glm::vec3& cell_pos);

            /** draws the cell within the current chunk */
            void draw(const Cell& c);

            void draw(const Chunk& c, const glm::vec3& chunk_pos);

    };


} // cell


#endif // CELL_RENDERER_H
