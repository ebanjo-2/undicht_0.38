#ifndef CELL_RENDERER_H
#define CELL_RENDERER_H

#include <core/renderer.h>
#include <camera/perspective_camera_3d.h>

#include <world/cell.h>
#include <world/chunk.h>
#include <world/world.h>

#include <rendering/texture_atlas.h>

namespace cell {

    class CellRenderer : public undicht::graphics::Renderer {

        public:
            // rendering to the geometry buffer

            undicht::graphics::Shader m_geometry_shader;

            undicht::graphics::Uniform m_view;
            undicht::graphics::Uniform m_proj;
            undicht::graphics::Uniform m_chunk_pos;

            undicht::graphics::Texture m_uv_texture;
            undicht::graphics::Texture m_normal_texture;
            undicht::graphics::Texture m_depth_texture;

            undicht::graphics::FrameBuffer m_geometry_buffer;

            void initGeometryStage();

        public:
            // rendering lights

        public:
            // rendering the final scene

            undicht::graphics::VertexBuffer m_screen_quad;

            undicht::graphics::Shader m_final_shader;

            void initFinalStage();

            void drawFinalScene();

        public:

            CellRenderer();
            virtual ~CellRenderer();

            void init();
            void term();

            virtual void clearFramebuffer();
            virtual void setViewport(int width, int height, int offset_x = 0, int offset_y = 0);

            void loadCam(undicht::graphics::PerspectiveCamera3D& player_cam);

            void loadTextureAtlas(const TextureAtlas& atlas);

            void draw(Chunk& c, const glm::ivec3& chunk_pos);

            void draw(World& w);



    };


} // cell


#endif // CELL_RENDERER_H
