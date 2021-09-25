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
            undicht::graphics::Uniform m_inv_view;

            undicht::graphics::Uniform m_proj;
            undicht::graphics::Uniform m_inv_proj;

            undicht::graphics::Uniform m_cam_pos;

            undicht::graphics::Uniform m_chunk_pos;

            undicht::graphics::Texture m_geo_texture;
            undicht::graphics::Texture m_depth_texture;

            undicht::graphics::FrameBuffer m_geometry_buffer;

            void initGeometryStage();

        public:
            // rendering lights

            undicht::graphics::Shader m_light_shader;

            undicht::graphics::Texture m_light_map;
            undicht::graphics::FrameBuffer m_light_buffer;

            undicht::graphics::VertexBuffer m_light_model;

			undicht::graphics::Uniform m_viewport_size;

            // light uniforms
			undicht::graphics::Uniform m_light_pos;
			undicht::graphics::Uniform m_light_color;
			undicht::graphics::Uniform m_light_radius;
			undicht::graphics::Uniform m_db_light_radius;

            void initLightStage();

            void drawLight(const glm::vec3& pos, const glm::vec3& color, float radius);

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
