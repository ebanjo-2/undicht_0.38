#ifndef CELL_RENDERER_H
#define CELL_RENDERER_H


#include <core/renderer.h>

#include <core/shader.h>

#include <camera/perspective_camera_3d.h>

#include <chunks/draw_chunk.h>
#include <material.h>

#include <files/model_loading/image_loader.h>
#include <core/texture.h>

namespace cell {

    class CellRenderer : public undicht::graphics::Renderer {

        public:

            // camera uniforms
            static undicht::graphics::Uniform* s_proj;
            static undicht::graphics::Uniform* s_view;

            // chunk uniforms
            static undicht::graphics::Uniform* s_chunk_offset;

            static undicht::graphics::Shader* s_shader;

            static int s_renderer_count;

            static void init();
            static void term();

        public:
            // drawing

            void loadCam(undicht::graphics::PerspectiveCamera3D& cam);

            void drawChunk(DrawChunk& chunk);

            CellRenderer();
            virtual ~CellRenderer();

    };

} // cell

#endif // CELL_RENDERER_H
