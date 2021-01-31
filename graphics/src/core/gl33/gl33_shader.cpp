#include <core/graphics_core.h>

#ifdef USE_GL33

#include "gl33_tools.h"
#include "event_logger.h"

#include <glad/glad.h>
#include "core/shader.h"
#include "core/texture.h"
#include "core/uniform.h"


using namespace undicht::tools;

namespace undicht {

    namespace graphics {

        namespace gl33 {

            unsigned int bound_shader = 0;

            Shader::Shader() {

                // creating the opengl shading program
                m_id = glCreateProgram();

            }

            Shader::~Shader() {

                if(m_id.removeUser()) {
                    glDeleteProgram(m_id);
                }

            }


            void Shader::loadSource(const std::string& source) {

                std::string vert_src, frag_src, geom_src;

                splitShaderSrc(source, vert_src, frag_src, geom_src);


                // creating the opengl shaders
                unsigned int vert_shader = createGLShader(vert_src, GL_VERTEX_SHADER);
                unsigned int frag_shader = createGLShader(frag_src, GL_FRAGMENT_SHADER);
                unsigned int geom_shader = createGLShader(geom_src, GL_GEOMETRY_SHADER);


                if(vert_shader) // there has to be one...
                    glAttachShader(m_id, vert_shader);

                if(frag_shader)
                    glAttachShader(m_id, frag_shader);

                if(geom_shader)
                    glAttachShader(m_id, geom_shader);

                glLinkProgram(m_id);

                glDeleteShader(vert_shader);
                glDeleteShader(frag_shader);
                glDeleteShader(geom_shader);

                undCheckGLError(UND_CODE_ORIGIN);

            }

            void Shader::loadUniform(const graphics::Uniform& u) {
                // loading the uniform to the shader

                //undCheckGLError(UND_CODE_ORIGIN);

                bind();

                if(!u.m_ubo_id) {

                    int location = glGetUniformLocation(m_id, u.getName().data());

                    int type = u.getType();


                    if(type == UND_INT) {
                        glUniform1i(location, *(int*)u.getData());
                    } else if(type == UND_FLOAT ) {
                        glUniform1f(location, *(float*)u.getData());
                    } else if(type == UND_VEC4F) {
                        glUniform4fv(location, 1, (float*)u.getData());
                    } else if(type == UND_VEC3F) {
                        glUniform3fv(location, 1, (float*)u.getData());
                    } else if(type == UND_VEC3I) {
                        glUniform3iv(location, 1, (int*)u.getData());
                    } else if(type == UND_VEC2F) {
                        glUniform2fv(location, 1, (float*)u.getData());
                    } else if(type == UND_MAT4F) {
                        glUniformMatrix4fv(location, 1, GL_FALSE, (float*)u.getData());
                    } else if(type == UND_MAT3F) {
                        glUniformMatrix3fv(location, 1, GL_FALSE, (float*)u.getData());
                    }

                } else {

                    //undCheckGLError(UND_CODE_ORIGIN);

                    // ubo (using the method from learnopengl.com, i have no idea how and if this works
                    // since i am to lazy rn to read and understand everything described there about ubos)

                    unsigned int uniform_block_index = glGetUniformBlockIndex(m_id, u.getName().data());

                    glUniformBlockBinding(m_id, uniform_block_index, 0);
                }



            }

            void Shader::loadTexture(const graphics::Texture& t) {


                if(t.m_type && t.m_id) {
                    // binding the texture and setting the sampler uniform in the shader

					bool texture_known;

					int texture_id = getTextureID(t.m_name, texture_known);
					t.bind(texture_id);

					if (!texture_known) {
						/** loading a unique id to the sampler in the shader */

						graphics::Uniform u;
						u.setData(&texture_id, UND_INT);

						u.setName(t.m_name);
						loadUniform(u);
					}

					

                } else {

                    EventLogger::storeNote(Note(UND_ERROR, "SHADER: ERROR: failed to load texture to shader: no type / id", UND_CODE_ORIGIN));
                }


            }


            /////////////////////////////////////// non api functions ///////////////////////////////////////


            int Shader::getTextureID(const std::string& texture_name, bool& was_known) {
				/** to load multiple textures to a shader, each sampler2D has to be given a unique value */

                // checking if the texture name already has an id associated with it
                for (unsigned int i = 0; i < m_textures.size(); i++) {

                    if(!m_textures.at(i).compare(texture_name)) {

						was_known = true;
                        return i;
                    }

                }


                // adding the texture to the list of textures loaded to the shader
                m_textures.push_back(texture_name);
	
				was_known = false;
                return m_textures.size() - 1;
            }


            void Shader::bind() {

                if(m_id != bound_shader) {

                    glUseProgram(m_id);
                    bound_shader = m_id;

                }

            }

            void Shader::bind(int id) {

                if(id != bound_shader) {

                    glUseProgram(id);
                    bound_shader = id;

                }

            }

            void Shader::splitShaderSrc(const std::string& source, std::string& vert_src, std::string& frag_src, std::string& geom_src ) {
                /// splits the shader source into glsl shaders

                size_t vert_start = source.find("#vertex");
                size_t frag_start = source.find("#fragment");
                size_t geom_start = source.find("#geometry");
                size_t source_end = source.size();

                if((vert_start == std::string::npos) || (frag_start == std::string::npos)) {
                    EventLogger::storeNote(Note(UND_ERROR, "SHADER:ERROR: failed to load shader src: no fragment or vertex shader", UND_CODE_ORIGIN));
                    return;
                }

                if(geom_start != std::string::npos) {
                    geom_src = source.substr(geom_start + 9, source_end - geom_start - 9);
                } else {
                    geom_start = source_end;
                }

                vert_src = source.substr(vert_start + 7, frag_start - vert_start - 7);
                frag_src = source.substr(frag_start + 9, geom_start - frag_start - 9);


            }

            unsigned int Shader::createGLShader(const std::string& source, unsigned int type) {

                unsigned int shader_id = glCreateShader(type);
                const char* src = source.data();
                const int src_size = source.length();

                if(!src_size)
                    return 0;

                glShaderSource(shader_id, 1, &src, &src_size);
                glCompileShader(shader_id);

                //check if compiling the Shader was successfull
                int  success;
                char info_log[512];
                glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

                if(!success) {
                    glGetShaderInfoLog(shader_id, 512, NULL, info_log);
                    EventLogger::storeNote(Note(UND_ERROR, toStr("ERROR: SHADER: ") + toStr(info_log), UND_CODE_ORIGIN));
                    return 0;
                }

                return shader_id;
            }


        } // gl33

    } // graphics

} // undicht



#endif // USE_GL33
