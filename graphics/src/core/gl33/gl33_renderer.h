#ifdef RENDERER_H // this file should only be included via core/renderer.h

#ifndef GL33_RENDERER_H
#define GL33_RENDERER_H


namespace undicht {

	namespace graphics {

		namespace gl33 {

			class Renderer : public interf::Renderer {

			public:
				// storing the last submissions

				VertexBuffer* m_current_vbo = 0;
				Shader* m_current_shader = 0;
				FrameBuffer* m_current_fbo = 0;

				bool m_depth_test_enabled = false;
				bool m_write_to_depth_buffer = false; // in case you want to use depth testing but dont want to write to the depth buffer
				int m_depth_test_operator = tools::UND_LESS;

				bool m_culling_enabled = false;
				int m_cull_face = tools::UND_BACK_FACE;

				bool m_blending_enabled = false;
				int m_blending_sfactor = tools::UND_SRC_ALPHA;
				int m_blending_dfactor = tools::UND_ONE_MINUS_SRC_ALPHA;

				int m_viewport_width = 800;
				int m_viewport_height = 600;
				int m_viewport_offset_x = 0;
				int m_viewport_offset_y = 0;

				// current state of opengl
				static bool s_depth_test_enabled;
				static bool s_write_to_depth_buffer;
				static int s_depth_test_operator;

				static bool s_culling_enabled;
				static int s_cull_face;

				static bool s_blending_enabled;
				static int s_blending_sfactor;
				static int s_blending_dfactor;

				static int s_viewport_width;
				static int s_viewport_height;
				static int s_viewport_offset_x;
				static int s_viewport_offset_y;

				static int s_texture_filtering;

			public:
				// opengl only functions

				/// binding the submitted fbo
				virtual void useFbo();

				/** sets the global rendering settings to what this renderer uses */
				virtual void setViewport();
				virtual void enableDepthTest();
				virtual void enableBackFaceCulling();
				virtual void enableBlending();

			public:

				virtual void submit(graphics::VertexBuffer* vbo);
				virtual void submit(graphics::Shader* shader);
				virtual void submit(graphics::FrameBuffer* fbo);

				virtual void clearFramebuffer(float r = 0.1f, float g = 0.1f, float b = 0.1f, float alpha = 1.0f);
				virtual void draw(unsigned int instance_count = 1);

				/** redundand calls (with no changes) should be ignored */
				virtual void setViewport(int width, int height, int offset_x = 0, int offset_y = 0);
				virtual void enableDepthTest(bool enable, bool write_to_buffer = true, int test_operator = tools::UND_LESS);
				virtual void enableBackFaceCulling(bool enable, int cull_face = tools::UND_BACK_FACE);
				virtual void enableBlending(bool enable, int sfactor, int dfactor);


				virtual void getViewport(int& width, int& height, int& offset_x, int& offset_y);

				/** sets the viewport without a renderer (following drawcalls will override the values) */
				static void setGlobalViewport(int width, int height, int offset_x = 0, int offset_y = 0); // used for example to clear framebuffers

				Renderer();
				virtual ~Renderer();

			};

		} // gl33

	} // graphics

} // undicht

#endif // GL33_RENDERER_H

#endif // RENDERER_H
