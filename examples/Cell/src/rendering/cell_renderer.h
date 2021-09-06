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

        public:

            CellRenderer();
            virtual ~CellRenderer();

            void init();
            void term();

            void loadCam(undicht::graphics::PerspectiveCamera3D& player_cam);

            void loadTextureAtlas(const TextureAtlas& atlas);

            void draw(Chunk& c, const glm::vec3& chunk_pos);

    };


} // cell


#endif // CELL_RENDERER_H
