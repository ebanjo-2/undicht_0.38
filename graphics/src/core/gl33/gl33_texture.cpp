#include <core/graphics_core.h>

#ifdef USE_GL33


#include "core/texture.h"
#include <glad/glad.h>
#include <array>
#include <types.h>
#include "gl33_tools.h"

using namespace undicht::tools;

namespace undicht {

    namespace graphics {

        namespace gl33 {

            std::array<int,16> bound_textures;

            Texture::Texture() {

                unsigned int id;
                glGenTextures(1, &id);
                m_id = id;
				m_type = GL_TEXTURE_2D;

            }

            Texture::~Texture() {

                if(m_id.removeUser()) {
                    glDeleteTextures(1, &m_id.getID());
                }

            }

            //////////////////////////////////////// managing the textures format /////////////////////////////////////

            void Texture::setPixelFormat(const tools::BufferLayout& format) {

                std::array<int,4> component_types = getComponentTypes(format); // the types of each color component
                setOpenglFormat(component_types);


            }


            void Texture::setSize(int width, int height, int depth) {

                m_width = width;
                m_height = height;
                m_depth = depth;

                m_size_set = true;
                updateFormat();

            }


            void Texture::getSize(int& width, int& height) {

                width = m_width;
                height = m_height;

            }

            void Texture::getSize(int& width, int& height, int& depth) {

                width = m_width;
                height = m_height;
                depth = m_depth;

            }

            void Texture::setUsesMipMap(bool use_mip_map) {

                // if the mipmap was generated it will be used

            }

            void Texture::setFilteringMethod(int min_filter, int mag_filter) {
                /// how to retrieve color from the texture if it doesnt align with the pixels on the screen

                bind();

                m_min_filter = min_filter;
                m_mag_filter = mag_filter;

                int gl_min_filter = getGLFilteringMethod(min_filter);
                int gl_mag_filter = getGLFilteringMethod(mag_filter);

                if(gl_min_filter == -1 || gl_mag_filter == -1) {
                    EventLogger::storeNote(Note(UND_ERROR, "invalid texture filter method", UND_CODE_ORIGIN));
                    return;
                }

                glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, gl_min_filter);
                glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, gl_mag_filter);
            }

            void Texture::setWrappingMethod(int method) {
                /// what to do if color data is requested outside the range of 0-1 for uv components

                bind();

                int gl_method = GL_REPEAT;

                if(method == UND_REPEAT) {
                    gl_method = GL_REPEAT;
                } else if (method == UND_CLAMP_TO_EDGE) {
                    gl_method = GL_CLAMP_TO_EDGE;
                }

                glTexParameteri(m_type, GL_TEXTURE_WRAP_S, gl_method);
                glTexParameteri(m_type, GL_TEXTURE_WRAP_T, gl_method);
                glTexParameteri(m_type, GL_TEXTURE_WRAP_R, gl_method);

            }

            ///////////////////////////////////////// managing the textures data //////////////////////////////////////////


            void Texture::setData(char* data, int byte_size, int depth, int mip_map_level, int offset) {

                if(!(m_size_set && m_layout_set)) {
                    EventLogger::storeNote(Note(UND_ERROR, "Texture: cant set data, format incomplete", UND_CODE_ORIGIN));
                    return;
                }

                glBindTexture(m_type, m_id);

                if(m_type == GL_TEXTURE_2D) {

                    glTexImage2D(m_type, 0, m_memory_format, m_width, m_height, 0, m_pixel_layout, GL_UNSIGNED_BYTE, data);
                    //glTexSubImage2D(m_type, mip_map_level, offset, 0, m_width, m_height, m_memory_format, GL_UNSIGNED_BYTE, data);

                } else if (m_type == GL_TEXTURE_2D_ARRAY) {

                    glTexSubImage3D(m_type, 0, 0, 0, depth, m_width, m_height, 1, m_pixel_layout, GL_UNSIGNED_BYTE, data);


                }

                undCheckGLError(UND_CODE_ORIGIN);



            }


            void Texture::generateMipMaps() {

                bind();
                glGenerateMipmap(m_type);

            }

            ////////////////////////////////// managing the access of the texture data in a shader/////////////////////////////////////

            void Texture::setName(const std::string& name) {

                m_name = name;

            }

            ////////////////////////////////// opengl only functions //////////////////////////////////




            void Texture::bind(unsigned int target) const{

                if(target >= bound_textures.size()) {
                    EventLogger::storeNote(Note(UND_ERROR, "TEXTURE:ERROR: failed to bind texture, target exceeds limit", UND_CODE_ORIGIN));
                    return;
                }

                if(!m_type) {
                    EventLogger::storeNote(Note(UND_ERROR, "TEXTURE:ERROR: texture to be bound has no type", UND_CODE_ORIGIN));
                    return;
                }

                if(bound_textures[target] != m_id) {

                    glActiveTexture(GL_TEXTURE0 + target);
                    glBindTexture(m_type, m_id);
                    bound_textures[target] = m_id;

                }

            }


            /** updates the texture to the set layout and size */
            void Texture::updateFormat() {

                if(m_size_set && m_layout_set) {

                    if(m_depth == 1) {
                        // 2D texture

                        m_type = GL_TEXTURE_2D;
                        bind();
                        glTexImage2D(GL_TEXTURE_2D, 0, m_memory_format, m_width, m_height, 0, m_pixel_layout, GL_UNSIGNED_BYTE, NULL);

                    } else {
                        // texture array / 3d texture / cubemap

                        m_type = GL_TEXTURE_2D_ARRAY;
                        bind();

                        // glTexStorage3D(m_type, 1, m_memory_format, m_width, m_height, m_depth); // might require opengl 4.2 or higher
                        glTexImage3D(m_type, 0, m_memory_format, m_width, m_height, m_depth, 0, m_pixel_layout, GL_UNSIGNED_BYTE, 0);

                    }

                    if(m_wrapping_method == -1) {
                        setWrappingMethod(UND_REPEAT);
                    }

                    if((m_mag_filter == -1) || (m_min_filter == -1)) {
                        setFilteringMethod(UND_LINEAR, UND_LINEAR);
                    }

                    undCheckGLError(UND_CODE_ORIGIN);

                }

            }

            /** @return an array containing the type for each component of the color */
            std::array<int,4> Texture::getComponentTypes(const tools::BufferLayout& format) {

                int component_count = 0;
                std::array<int,4> component_types; // the types of each color component

                // initializing all without a type
                for(int& i : component_types)
                    i = UND_TYPE_UNAVAILABLE;

                for(const int& component : format.m_types) {


                    if(component == UND_VEC3F) { // 3 floats
                        component_types[component_count + 0] = UND_FLOAT;
                        component_types[component_count + 1] = UND_FLOAT;
                        component_types[component_count + 2] = UND_FLOAT;
                        component_count += 3;
                    } else if(component == UND_VEC2F) { // 2 floats
                        component_types[component_count + 0] = UND_FLOAT;
                        component_types[component_count + 1] = UND_FLOAT;
                        component_count += 2;
                    } else {
                        // types defining one component

                        component_types[component_count] = component;
                        component_count++;
                    }
                }

                return component_types;
            }

            void Texture::setOpenglFormat(const std::array<int,4>& component_types) {

				if (component_types[3] != UND_TYPE_UNAVAILABLE) {
					// rgba channels

					if (component_types[0] == UND_UNSIGNED_CHAR) {
						// 8 bits per color channel
						m_pixel_layout = GL_RGBA;
						m_memory_format = GL_RGBA8;
					} else if (component_types[0] == UND_UINT16) {
						// 16 bits per color channel
						m_pixel_layout = GL_RGBA;
						m_memory_format = GL_RGBA16;
					}
					else if (component_types[0] == UND_UINT2) {
						// 2 bits per color channel
						m_pixel_layout = GL_RGBA;
						m_memory_format = GL_RGBA2;
					}

				}
				else if (component_types[2] != UND_TYPE_UNAVAILABLE) {
					// rgb channels

					if (component_types[0] == UND_UNSIGNED_CHAR) {
						// 8 bits per color channel
						m_pixel_layout = GL_RGB;
						m_memory_format = GL_RGB8;
					}
					else if (component_types[0] == UND_UINT16) {
						// 16 bits per color channel
						m_pixel_layout = GL_RGB;
						m_memory_format = GL_RGB16;
					}

				}
				else if (component_types[1] != UND_TYPE_UNAVAILABLE) {
					// rg channels

					if (component_types[0] == UND_UNSIGNED_CHAR) { // equals UND_UINT8
						// 8 bits per color channel
						m_pixel_layout = GL_RG;
						m_memory_format = GL_RG8;
					}

				}
				else if (component_types[0] != UND_TYPE_UNAVAILABLE) {
					// single channel

					if (component_types[0] == UND_UNSIGNED_CHAR) { // equals UND_UINT8
						// 8 bits per color channel
						m_pixel_layout = GL_RED;
						m_memory_format = GL_R8;
					}
					else if (component_types[0] == UND_UINT16) {
						// 16 bits per color channel

						m_pixel_layout = GL_RED_INTEGER;
						m_memory_format = GL_R16UI;
					}

				}

                if(m_pixel_layout && m_memory_format) {

                    m_layout_set = true;
                    updateFormat();
                } else {

                    EventLogger::storeNote(Note(UND_ERROR, "TEXTURE:ERROR: no known format", UND_CODE_ORIGIN));
                }

            }

            void Texture::setOpenglFormat(int layout, int memory_format) {

                m_pixel_layout = layout;
                m_memory_format = memory_format;

                m_layout_set = true;
                updateFormat();
            }

            int Texture::getGLFilteringMethod(int und_filtering_method) {
                /// translates an undicht filtering type to an opengl one

                if(und_filtering_method == UND_NEAREST) {

                    return GL_NEAREST;
                } else if (und_filtering_method == UND_LINEAR) {

                    return GL_LINEAR;
                }

				return -1;
            }


        } // gl33

    } // graphics

} // undicht

#endif // USE_GL33
