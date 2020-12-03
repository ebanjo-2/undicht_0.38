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
