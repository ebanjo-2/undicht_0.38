#ifdef USE_GL33

#include <glad/glad.h>

#include "gl33_frame_buffer.h"
#include <iostream>
#include <core/event_logger.h>
#include "gl_tools.h"
#include <core/buffer_layout.h>

using namespace undicht::core;

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

                std::cout << "resizing the framebuffer to " << width << " " << height << "\n";


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
                for(int i = 0; i < m_attachments.size(); i++) {

                    // for sending an graphics::Texture to the function
                    graphics::Texture graphics_texture;
                    graphics_texture.m_shared_lib_object = m_attachments.at(i);
                    int type = m_attachment_types.at(i);

                    // the attachment gets stored again
                    m_attachments.erase(m_attachments.begin() + i);
                    m_attachment_types.erase(m_attachment_types.begin() + i);


                    addAttachment(graphics_texture, type);
                }

            }

            void FrameBuffer::getSize(int& width, int& height) {

                width = m_width;
                height = m_height;

            }


            void FrameBuffer::addAttachment(graphics::Texture& texture, int attachment_type) {
                /** @param texture: the framebuffer can only be used while the texture object exists
                * @param texture: it should have a pixel layout set before attachment, if not, default 24 bit rgb is going to be used for color attachments
                * @param attachment_type: UND_COLOR_ATTACHMENT, UND_DEPTH_ATTACHMENT_WRITE_ONLY or UND_DEPTH_ATTACHMENT_READ_AND_WRITE */

                gl33::Texture* opengl_texture = (gl33::Texture*)texture.m_shared_lib_object;

                if(attachment_type == UND_COLOR_ATTACHMENT) {
                    // attaching a texture that can store the color output of a fragment shader
                    if(!opengl_texture->m_layout_set) {
                        // setting default 24 bit rgb layout for the pixels
                        opengl_texture->setPixelFormat(BufferLayout({UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR}));
                    }

                    opengl_texture->setSize(m_width, m_height, 1);

                    bind(); // binding the Framebuffer
                    opengl_texture->bind(); // binding the texture
                    opengl_texture->setData(0, 0);// reserving memory
                    std::cout << "adding color attachment " << getColorOutputCount() << "\n";
                    glFramebufferTexture2D(m_type, GL_COLOR_ATTACHMENT0 + getColorOutputCount(), GL_TEXTURE_2D, opengl_texture->m_id, 0);

                } else if (attachment_type == UND_DEPTH_ATTACHMENT_READ_AND_WRITE) {
                    // attaching a texture that can store the depth values of a scene
                    opengl_texture->setOpenglFormat(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT);
                    opengl_texture->setSize(m_width, m_height, 1);
                    opengl_texture->setData(0, 0);// reserving memory

                    bind(); // binding the Framebuffer
                    opengl_texture->bind(); // binding the texture
                    std::cout << "adding depth attachment" << "\n";
                    glFramebufferTexture2D(m_type, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, opengl_texture->m_id, 0);

                } else if (attachment_type == UND_DEPTH_ATTACHMENT_WRITE_ONLY) {
                    // using a renderbuffer, may be faster then a texture

                    // to be done (needs some changes in the texture class)
                } else {
                    EventLogger::storeNote(Note(UND_ERROR, "FrameBuffer: ERROR: cant add attachment, wrong attachment_type", UND_CODE_ORIGIN));
                    return;
                }

                m_attachments.push_back(opengl_texture);
                m_attachment_types.push_back(attachment_type);
                updateColorOutputs();

                undCheckGLError(UND_CODE_ORIGIN);

                checkStatus();

            }


            void FrameBuffer::removeAttachment(graphics::Texture& texture) {
                /** @brief removes the attachment from the framebuffer */

            }

            void FrameBuffer::clearAttachments() {

                //std::cout << "clearing the framebuffer" << "\n";

                checkStatus();

                bind();
                glViewport(0, 0, m_width, m_height);

                glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                undCheckGLError(UND_CODE_ORIGIN);

            }


            //////////////////////////////////////////////////// opengl only functions ///////////////////////////////////////////

            void FrameBuffer::updateColorOutputs() {
                // taken from undicht 0.21.9

                // counting the number of enabled color outputs
                int number_of_outputs = getColorOutputCount();

                // saving the index of each enabled output in an array
                unsigned int* outputs = new unsigned int[number_of_outputs]; // an array that holds all enabled GL_COLOR_ATTACHMENTS
                int last_enabled_output = 0;
                for(unsigned int i = 0; i < m_attachment_types.size(); i++) {
                    if(m_attachment_types.at(i) == UND_COLOR_ATTACHMENT) {
                        outputs[last_enabled_output] = GL_COLOR_ATTACHMENT0 + i;
                        last_enabled_output++;
                    }
                }

                if(number_of_outputs) {
                    glDrawBuffers(number_of_outputs, outputs);
                } else {
                    // from undicht 0.05
                    glDrawBuffer(GL_NONE);// no color attachment will be used
                    glReadBuffer(GL_NONE);// i.e when rendering only to a depth buffer
                }

                delete[] outputs;
            }

            int FrameBuffer::getColorOutputCount() {
                // counting the number of enabled color outputs

                int number_of_outputs = 0;
                for(int &output_type : m_attachment_types) {
                    if(output_type == UND_COLOR_ATTACHMENT)
                        number_of_outputs += 1;
                }

                return number_of_outputs;
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

            /////////////////////////////////// functions to load Framebuffer objects from the shared lib ////////////////////////////////

            SHARED_LIB_EXPORT implementation::FrameBuffer*createFrameBuffer() {
                return new gl33::FrameBuffer;
            }

            SHARED_LIB_EXPORT void copyFrameBuffer(implementation::FrameBuffer* c, implementation::FrameBuffer* o) {
                *(FrameBuffer*)c = *(FrameBuffer*)o;
            }

            SHARED_LIB_EXPORT void deleteFrameBuffer(implementation::FrameBuffer* object) {
                delete (FrameBuffer*)object;
            }


        } // gl33

    } // graphics

} // undicht

#endif // USE_GL33
