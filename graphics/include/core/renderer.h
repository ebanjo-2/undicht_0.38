#ifndef RENDERER_H
#define RENDERER_H

#include <core/shared_lib_class.h>
#include <graphics/shader.h>
#include <graphics/vertex_buffer.h>
#include <graphics/frame_buffer.h>

namespace undicht {

    namespace graphics {

        class GraphicsLib;

        SHARED_LIB_DECL_BASE_CLASS(Renderer, RendererBase, createRenderer, copyRenderer, deleteRenderer);

        SHARED_LIB_CLASS(class Renderer : public RendererBase{
            /// the class that can draw stuff
        private:
            // pointer to the implementation of the static functions
            static std::function<void(int,int,int,int)> s_set_viewport;
            static std::function<void(bool)> s_enable_depth_test;
            static std::function<void(bool)> s_enable_back_face_culling;
            static std::function<void(int&,int&,int&,int&)> s_get_viewport;

            friend GraphicsLib;

        public:

            virtual void submit(graphics::VertexBuffer* vbo);
            virtual void submit(graphics::Shader* shader);
            virtual void submit(graphics::FrameBuffer* fbo); // submit (FrameBuffer*) 0 to reset the submitted framebuffer

            // clears the content of the currently submitted framebuffer
            virtual void clearFramebuffer(float r = 0.1, float g = 0.2, float b = 0.3, float alpha = 1);

            virtual void draw(unsigned int instance_count = 1);

            /** redundand calls (with no changes) should be ignored */
            static void setViewport(int width, int height, int offset_x = 0, int offset_y = 0);
            static void enableDepthTest(bool enable = true);
            static void enableBackFaceCulling(bool enable = true);

            static void getViewport(int& width, int& height, int& offset_x, int& offset_y);

            Renderer();
            virtual ~Renderer();

        };)


    } // graphics

} // undicht

#endif // RENDERER_H
