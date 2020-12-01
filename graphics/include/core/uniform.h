#ifndef UNIFORM_H
#define UNIFORM_H

#include <core/graphics_core.h>
#include <types.h>
#include <buffer_layout.h>
#include <string>



namespace undicht {

    namespace graphics {

        namespace interf {

            class Uniform {
                    /** uniforms are used to load variables to a shader
                    * this includes simple types such as ints, floats, ... and buffers
                    * the uniforms can be accessed in the shader via their names */
                public:
                    // all data should be set before loading the uniform to a shader

                    /** the uniform stores a single type,
                    * see core/types.h for options
                    * @param copy data: if true, the data gets copied into a pointer private to the uniform
                    * if false, the given data pointer will be kept as the data storage (so dont delete it) */
                    virtual void setData(const void* data, int type, bool copy_data = true) = 0;

                    /** stores the data in a buffer accessable in the shader */
                    virtual void setData(const void* data, core::BufferLayout layout, int num_of_elements) = 0;

                    /** sets the name with which the uniforms data can be accessed in the shader */
                    virtual void setName(const std::string& name) = 0;

                public:
                    // functions to retrieve data

                    virtual const std::string& getName() const = 0;
                    virtual int getType() const = 0;
                    virtual const void* getData() const = 0;
                    virtual const core::BufferLayout& getLayout() const = 0;


                    Uniform() = default;
                    virtual ~Uniform() = default;

            };

        } // interf

        #ifdef USE_GL33
        #include <core/gl33/gl33_uniform.h>
        typedef gl33::Uniform Uniform;
        #endif // USE_GL33

    } // graphics

} // undicht

#endif // UNIFORM_H
