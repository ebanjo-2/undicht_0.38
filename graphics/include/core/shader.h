#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "graphics_core.h"
#include "uniform.h"
#include "texture.h"

namespace undicht {

    namespace graphics {

        namespace interf {

        class Shader {
            public:

                Shader() = default;
                virtual ~Shader() = default;

            public:
                // loading the shader source

                /// glsl, but in a single string/file, shader types marked with #vertex, #fragment, ...
                /// similar to TheChernos solution in the Hazel engine
                virtual void loadSource(const std::string& source) = 0;

            public:
                // loading data to the shader

                virtual void loadUniform(const graphics::Uniform& u) = 0;

                virtual void loadTexture(const graphics::Texture& t) = 0;


        };

        } // interf

#ifdef USE_GL33
#include <core/gl33/gl33_shader.h>
typedef gl33::Shader Shader;
#endif // USE_GL33

    } // graphics

} // undicht

#endif // SHADER_H
