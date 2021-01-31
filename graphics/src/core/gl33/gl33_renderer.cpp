#include "core/graphics_core.h"

#ifdef USE_GL33 

#include <glad/glad.h>

#include "core/renderer.h"
#include "event_logger.h"

using namespace undicht::tools;

namespace undicht {

    namespace graphics {

        namespace gl33 {

            bool Renderer::s_depth_test_enabled = false;
            bool Renderer::s_culling_enabled = false;

            int Renderer::s_viewport_width = 0;
            int Renderer::s_viewport_height = 0;
            int Renderer::s_viewport_offset_x = 0;
            int Renderer::s_viewport_offset_y = 0;

            Renderer::Renderer() {
                //ctor
            }

            Renderer::~Renderer() {
                //dtor
            }

            void Renderer::submit(graphics::VertexBuffer* vbo) {

                m_current_vbo = vbo;


            }

            void Renderer::submit(graphics::Shader* shader) {

                m_current_shader = shader;

            }

            void Renderer::submit(graphics::FrameBuffer* fbo) {

                if(fbo) {

                    m_current_fbo = fbo;
                    m_current_fbo->checkStatus();
                } else {
                    m_current_fbo = 0;
                }

            }

            void Renderer::clearFramebuffer(float r, float g, float b, float alpha) {

                if(m_current_fbo) {
                    m_current_fbo->clearAttachments();
                } else {
                    FrameBuffer::bind(0);
                    glClearColor(r,g,b,alpha);

                    if(s_depth_test_enabled) {

                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    } else {

                        glClear(GL_COLOR_BUFFER_BIT);
                    }

                }


            }


            void Renderer::draw(unsigned int instance_count) {

				/** setting the global vars to what this renderer uses */
				setViewport();
				enableBackFaceCulling();
				enableDepthTest();

                if(m_current_shader) {

                    m_current_shader->bind();

                } else {

                    EventLogger::storeNote(Note(UND_ERROR, "ERROR: draw call failed, no shader submitted", UND_CODE_ORIGIN));
                    return;
                }

                if(m_current_vbo) {

                    m_current_vbo->bind();
                } else {

                    EventLogger::storeNote(Note(UND_ERROR, "ERROR: draw call failed, no vertex buffer submitted", UND_CODE_ORIGIN));
                    return;
                }

                if(!m_current_vbo->getVertexSize()) {

                    EventLogger::storeNote(Note(UND_ERROR, "ERROR: draw call failed, vertex buffer has no layout", UND_CODE_ORIGIN));
                    return;
                }

                useFbo();

                if(instance_count == 1) {

                    if(m_current_vbo->getUsesIndices()) {

                        glDrawElements(GL_TRIANGLES, m_current_vbo->getIndexBufferSize() / sizeof(int), GL_UNSIGNED_INT, 0);
                    } else {

                        glDrawArrays(GL_TRIANGLES, 0, m_current_vbo->getSize() / m_current_vbo->getVertexSize());
                    }


                } else {

                    if(m_current_vbo->getUsesIndices()) {
                        // has not been tested

                        glDrawElementsInstanced(GL_TRIANGLES, m_current_vbo->getIndexBufferSize() / sizeof(int), GL_UNSIGNED_INT, 0, instance_count);
                    } else {

                        glDrawArraysInstanced(GL_TRIANGLES, 0, m_current_vbo->getSize() / m_current_vbo->getVertexSize(), instance_count);
                    }

                }

            }

            void Renderer::setViewport(int width, int height, int offset_x, int offset_y) {

				m_viewport_width = width;
				m_viewport_height = height;
				m_viewport_offset_x = offset_x;
				m_viewport_offset_y = offset_y;
            }

            void Renderer::enableDepthTest(bool enable) {

				m_depth_test_enabled = enable;
            }

            void Renderer::enableBackFaceCulling(bool enable) {

				m_culling_enabled = enable;
            }


            void Renderer::getViewport(int& width, int& height, int& offset_x, int& offset_y) {

                width = s_viewport_width;
                height = s_viewport_height;
                offset_x = s_viewport_offset_x;
                offset_y = s_viewport_offset_y;

            }


			void Renderer::setGlobalViewport(int width, int height, int offset_x, int offset_y) {
				/** sets the viewport without a renderer (following drawcalls will override the values) */

				if ((width != s_viewport_width) || (height != s_viewport_height) || (offset_x != s_viewport_offset_x) || (offset_y != s_viewport_offset_y)) {

					glViewport(offset_x, offset_y, width, height);

					s_viewport_width = width;
					s_viewport_height = height;
					s_viewport_offset_x = offset_x;
					s_viewport_offset_y = offset_y;
				}
			}

            /////////////////////////////////////////////////// opengl only functions //////////////////////////////////////////


            void Renderer::useFbo() {
                /// binding the submitted fbo

                if(m_current_fbo) {
                    m_current_fbo->bind();
                } else {
                    // using the default framebuffer
                    FrameBuffer::bind(0);
                }

            }

			void Renderer::setViewport() {

				if ((m_viewport_width != s_viewport_width) || (m_viewport_height != s_viewport_height) || (m_viewport_offset_x != s_viewport_offset_x) || (m_viewport_offset_y != s_viewport_offset_y)) {

					glViewport(m_viewport_offset_x, m_viewport_offset_y, m_viewport_width, m_viewport_height);

					s_viewport_width = m_viewport_width;
					s_viewport_height = m_viewport_height;
					s_viewport_offset_x = m_viewport_offset_x;
					s_viewport_offset_y = m_viewport_offset_y;
				}

			}

			void Renderer::enableDepthTest() {


				if (s_depth_test_enabled != m_depth_test_enabled) {

					if (m_depth_test_enabled) {
						glEnable(GL_DEPTH_TEST);
					}
					else {
						glDisable(GL_DEPTH_TEST);
					}

					s_depth_test_enabled = m_depth_test_enabled;
				}



			}

			void Renderer::enableBackFaceCulling() {

				if (s_culling_enabled != m_culling_enabled) {

					if (m_culling_enabled) {
						glEnable(GL_CULL_FACE);
					}
					else {
						glDisable(GL_CULL_FACE);
					}

					s_culling_enabled = m_culling_enabled;
				}

			}
        } // gl33

    } // graphics

} // undicht


#endif // USE_GL33