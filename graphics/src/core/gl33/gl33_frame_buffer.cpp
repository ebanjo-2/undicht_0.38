#include "core/graphics_core.h"

#ifdef USE_GL33

#include <glad/glad.h>
#include <iostream>


#include "core/frame_buffer.h"
#include "gl33_tools.h"

#include "event_logger.h"
#include "buffer_layout.h"

#include "core/renderer.h"

using namespace undicht::tools;

namespace undicht {

    namespace graphics {

        namespace gl33 {

            unsigned int bound_fbo = 0;

            FrameBuffer::FrameBuffer() : m_type(GL_FRAMEBUFFER) {

                unsigned int id;
                glGenFramebuffers(1, &id);
                m_id = id;

            }

            FrameBuffer::~FrameBuffer() {

                if(m_id.removeUser()) {
                    glDeleteFramebuffers(1, &m_id.getID());
                }
            }

            void FrameBuffer::setSize(int width, int height) {
                /** setting the size of the framebuffer and all of its attachments */

                if((width == m_width) && (height == m_height)) {
                    return;
                }

                m_width = width;
                m_height = height;

                //std::cout << "resizing the framebuffer to " << width << " " << height << "\n";


                // according to this: https://gamedev.stackexchange.com/questions/91991/resizing-a-framebuffer-object-ie-its-attachments-on-screen-resize
                // the framebuffer should be deleted and a new one with the desired size should be created

                if(m_attachments.size()) {

                    if(m_id.removeUser()) {
                        glDeleteFramebuffers(1, &m_id.getID());
                    }

                    unsigned int id;
                    glGenFramebuffers(1, &id);
                    m_id = id;
                }

                // reattaching them to the new fbo

				std::vector<Texture*> att = m_attachments;
				std::vector<int> att_types = m_attachment_types;
				std::vector<int> color_locations = m_color_outputs;

				m_color_outputs.clear();
				m_attachments.clear();
				m_attachment_types.clear();

				int col_out_count = 0;

                for(unsigned int i = 0; i < att.size(); i++) {

					if (att_types[i] == UND_COLOR_ATTACHMENT) {

						addAttachment(*att[i], att_types[i], color_locations[col_out_count]);
						col_out_count++;
					}
					else {

						addAttachment(*att[i], att_types[i]);
					}
                }

            }

            void FrameBuffer::getSize(int& width, int& height) {

                width = m_width;
                height = m_height;

            }


            void FrameBuffer::addAttachment(graphics::Texture& texture, int attachment_type, int location) {
				/** @param texture: the framebuffer can only be used while the texture object exists
				* @param texture: it should have a pixel layout set before attachment, if not, default 24 bit rgb is going to be used for color attachments
				* @param attachment_type: UND_COLOR_ATTACHMENT, UND_DEPTH_ATTACHMENT_WRITE_ONLY or UND_DEPTH_ATTACHMENT_READ_AND_WRITE
				* @param location: the location under which the output buffer can be accessed in a shader */

                if(attachment_type == UND_COLOR_ATTACHMENT) {
                    // attaching a texture that can store the color output of a fragment shader

                    if(!texture.m_layout_set) {
                        // setting default 24 bit rgb layout for the pixels
						texture.setPixelFormat(BufferLayout({UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR}));
						texture.setFilteringMethod(UND_NEAREST, UND_NEAREST);
						texture.setWrappingMethod(UND_REPEAT);
                    }

					texture.setSize(m_width, m_height, 1);

                    bind(); // binding the Framebuffer
					texture.bind(); // binding the texture

					glFramebufferTexture2D(m_type, GL_COLOR_ATTACHMENT0 + location, GL_TEXTURE_2D, texture.m_id, 0);

					// updating the m_color_outputs vector
					// first: checking if the location was already used by another attachment
					bool color_output_stored = false;
					for (int& i : m_color_outputs) {

						if (i == location) {
							color_output_stored = true;
						}
					}

					// second: if the location is new, it gets added to the list
					if (!color_output_stored) m_color_outputs.push_back(location);

                } else if (attachment_type == UND_DEPTH_ATTACHMENT_READ_AND_WRITE) {
                    // attaching a texture that can store the depth values of a scene

					texture.setSize(m_width, m_height, 1);

					if (!texture.m_layout_set) {
						texture.setOpenglFormat(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT);
					}

                    bind(); // binding the Framebuffer
					texture.bind(); // binding the texture

					glFramebufferTexture2D(m_type, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.m_id, 0);

                } else if (attachment_type == UND_DEPTH_ATTACHMENT_WRITE_ONLY) {
                    // using a renderbuffer, may be faster then a texture

                    // to be done (needs some changes in the texture class)
                } else {
                    EventLogger::storeNote(Note(UND_ERROR, "FrameBuffer: ERROR: cant add attachment, wrong attachment_type", UND_CODE_ORIGIN));
                    return;
                }

                m_attachments.push_back(&texture);
                m_attachment_types.push_back(attachment_type);
                updateColorOutputs();

                undCheckGLError(UND_CODE_ORIGIN);

                checkStatus();

            }


            void FrameBuffer::removeAttachment(graphics::Texture& texture) {
                /** @brief removes the attachment from the framebuffer */

            }

            void FrameBuffer::clearAttachments(float r, float g, float b, float alpha) {

				// std::cout << "clearing fbo" << "\n";

                bind();

				Renderer::setGlobalViewport(m_width, m_height);

                glClearColor(r,g,b,alpha);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                undCheckGLError(UND_CODE_ORIGIN);

            }


            //////////////////////////////////////////////////// opengl only functions ///////////////////////////////////////////

            void FrameBuffer::updateColorOutputs() {
                // taken from undicht 0.21.9

                // saving the index of each enabled output in an array
                unsigned int* outputs = new unsigned int[m_color_outputs.size()]; // an array that holds all enabled GL_COLOR_ATTACHMENTS

				for (int i = 0; i < m_color_outputs.size(); i++) {
					outputs[i] = GL_COLOR_ATTACHMENT0 + m_color_outputs[i];
				}

				bind();

                if(m_color_outputs.size()) {

                    glDrawBuffers(m_color_outputs.size(), outputs);
                } else {
                    // from undicht 0.05
                    glDrawBuffer(GL_NONE);// no color attachment will be used
                    glReadBuffer(GL_NONE);// i.e when rendering only to a depth buffer
                }

                delete[] outputs;
            }


            int FrameBuffer::getColorOutputCount() {
                // counting the number of enabled color outputs

                return m_color_outputs.size();
            }

            void FrameBuffer::checkStatus() {
                // prints an Error message if the fbo is not ready to use

                bind();
                unsigned int status = glCheckFramebufferStatus(m_type);
                switch(status) {
                    case GL_FRAMEBUFFER_COMPLETE:
                        break;// no Error to be printed
                    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                        EventLogger::storeNote(Note(UND_WARNING, "WARNING: Framebuffer: incomplete, missing attachment", UND_CODE_ORIGIN));
                        break;
					case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
						EventLogger::storeNote(Note(UND_WARNING, "WARNING: Framebuffer: attached buffer is incomplete", UND_CODE_ORIGIN));
						break;
                    default:
                        EventLogger::storeNote(Note(UND_WARNING, "WARNING: Framebuffer: not ready to use", UND_CODE_ORIGIN));
                }

            }

            void FrameBuffer::bind() {

                bind(m_id);

            }

            void FrameBuffer::bind(int id) {

                if(id != bound_fbo) {

                    glBindFramebuffer(GL_FRAMEBUFFER, id);
                    bound_fbo = id;

                }

            }

        } // gl33

    } // graphics

} // undicht

#endif // USE_GL33
