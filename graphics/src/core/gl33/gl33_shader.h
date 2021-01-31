#ifdef SHADER_H // this file should only be included via include/core/shader.h

#ifndef GL33_SHADER_H
#define GL33_SHADER_H

#include "shared_id.h"

namespace undicht {

	namespace graphics {

		namespace gl33 {

			class Shader : public interf::Shader {
			public:

				tools::SharedID m_id = 0;

				std::vector<std::string> m_textures; // storing the name of each texture loaded to the shader

				Shader();
				virtual ~Shader();

				/// glsl, but in a single string/file, shader types marked with #vertex, #fragment, ...
				/// similar to TheChernos solution in the Hazel engine
				virtual void loadSource(const std::string& source);

			public:
				// loading data to the shader

				virtual void loadUniform(const Uniform& u);

				virtual void loadTexture(const graphics::Texture& t);

			public:
				// not api functions

				/** @param was_know: set to true, if the texture name was used before */
				virtual int getTextureID(const std::string& texture_name, bool& was_known);

				virtual void bind();

				static void bind(int id);

				/// splits the shader source into glsl shaders
				void splitShaderSrc(const std::string& source, std::string& vertex_src, std::string& fragment_src, std::string& geometry_src);

				unsigned int createGLShader(const std::string& source, unsigned int type);

			};

		} // gl33

	} // graphics

} // undicht

#endif // GL33_SHADER_H

#endif // SHADER_H
