#ifndef CELL_LIGHT_RENDERER_H
#define CELL_LIGHT_RENDERER_H

#include <core/renderer.h>

#include <camera/perspective_camera_3d.h>

namespace cell {

    class CellLightRenderer : public undicht::graphics::Renderer {

        public:
            // graphics objects

            undicht::graphics::Shader m_shader;
            undicht::graphics::Texture m_light_map;
            undicht::graphics::FrameBuffer m_light_buffer;

            undicht::graphics::VertexBuffer m_light_model;

        public:
            // uniforms

            undicht::graphics::Uniform m_viewport_size;

            // camera related
            undicht::graphics::Uniform m_view;
            undicht::graphics::Uniform m_proj;

            undicht::graphics::Uniform m_inv_view;
            undicht::graphics::Uniform m_inv_proj;

            undicht::graphics::Uniform m_cam_pos;

            // light uniforms
			undicht::graphics::Uniform m_light_pos;
			undicht::graphics::Uniform m_light_color;
			undicht::graphics::Uniform m_light_radius;
			undicht::graphics::Uniform m_db_light_radius;

        public:

            CellLightRenderer();
            CellLightRenderer(undicht::graphics::Texture& depth_map);
            virtual ~CellLightRenderer();

            virtual void init(undicht::graphics::Texture& depth_map);

            virtual void loadCam(undicht::graphics::PerspectiveCamera3D& player_cam);

            /** the name of the geometry_texture should be "geo_texture" */
            virtual void loadGeometry(const undicht::graphics::Texture& geometry_texture, const undicht::graphics::Texture& depth_map);

        public:
            // draw functions

            virtual void setViewport(int width, int height, int offset_x = 0, int offset_y = 0);

            virtual void draw(const glm::vec3& pos, const glm::vec3& color, float radius);

    };


} // cell


#endif // CELL_LIGHT_RENDERER_H
