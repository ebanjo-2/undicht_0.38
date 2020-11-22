#ifndef SHADER_H
#define SHADER_H

#include <core/shared_lib_class.h>
#include <string>
#include <graphics/uniform.h>
#include <graphics/texture.h>

namespace undicht {

    namespace graphics {

        SHARED_LIB_DECL_BASE_CLASS(Shader, ShaderBase, createShader, copyShader, deleteShader);

        SHARED_LIB_CLASS(class Shader : public ShaderBase {
            public:
                Shader();
                virtual ~Shader();

            public:
                // loading the shader source

                /// glsl, but in a single string/file, shader types marked with #vertex, #fragment, ...
                /// similar to TheChernos solution in the Hazel engine
                virtual void loadSource(const std::string& source);

            public:
                // loading data to the shader

                virtual void loadUniform(const graphics::Uniform& u);

                virtual void loadTexture(const graphics::Texture& t);


        };)

    } // graphics

} // undicht

#endif // SHADER_H
