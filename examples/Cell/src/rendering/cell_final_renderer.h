#ifndef CELL_FINAL_RENDERER_H
#define CELL_FINAL_RENDERER_H

#include <core/renderer.h>

#include <camera/perspective_camera_3d.h>


namespace cell {

    class CellFinalRenderer : public undicht::graphics::Renderer {

        public:
            // graphics objects

            undicht::graphics::Shader m_shader;
            undicht::graphics::VertexBuffer m_screen_quad;

        public:
            // uniforms

            undicht::graphics::Uniform m_view;
            undicht::graphics::Uniform m_proj;

            undicht::graphics::Uniform m_inv_view;
            undicht::graphics::Uniform m_inv_proj;

        public:

            CellFinalRenderer();
            virtual ~CellFinalRenderer();

            virtual void init();

            virtual void loadCam(undicht::graphics::PerspectiveCamera3D& player_cam);

            /** the name of the geometry_texture should be "geo_texture" */
            virtual void loadGeometry(const undicht::graphics::Texture& geometry_texture, const undicht::graphics::Texture& depth_map);

            virtual void loadLightMap(const undicht::graphics::Texture& light_map);

            virtual void loadTextureAtlas(const undicht::graphics::Texture& texture_atlas);

        public:
            // draw functions

            virtual void draw();

    };


} // cell


#endif // CELL_FINAL_RENDERER_H
