#ifndef RENDERER_H
#define RENDERER_H

#include "core/graphics_core.h"
#include "core/shader.h"
#include "core/vertex_buffer.h"
#include "core/frame_buffer.h"

namespace undicht {

    namespace graphics {

		namespace interf {

			class Renderer {
				/// the class that can draw stuff

			public:

				virtual void submit(graphics::VertexBuffer* vbo) = 0;
				virtual void submit(graphics::Shader* shader) = 0;
				virtual void submit(graphics::FrameBuffer* fbo) = 0; // submit (FrameBuffer*) 0 to reset the submitted framebuffer

				// clears the content of the currently submitted framebuffer
				virtual void clearFramebuffer(float r = 0.1, float g = 0.2, float b = 0.3, float alpha = 1) = 0;

				virtual void draw(unsigned int instance_count = 1) = 0;

				/** redundand calls (with no changes) should be ignored */
				void setViewport(int width, int height, int offset_x = 0, int offset_y = 0);
				void enableDepthTest(bool enable);
				void enableBackFaceCulling(bool enable);

				void getViewport(int& width, int& height, int& offset_x, int& offset_y);

				/** sets the viewport without a renderer (following drawcalls will override the values) */
				static void setGlobalViewport(int width, int height, int offset_x = 0, int offset_y = 0); // used for example to clear framebuffers


				Renderer() = default;
				virtual ~Renderer() = default;

			};

		} // interf

    } // graphics

} // undicht


#ifdef USE_GL33

#include "core/gl33/gl33_renderer.h"

namespace undicht {
	namespace graphics {
		typedef gl33::Renderer Renderer;
	} // graphics
} // undicht

#endif // USE_GL33


#endif // RENDERER_H
