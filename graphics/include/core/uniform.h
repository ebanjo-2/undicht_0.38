#ifndef UNIFORM_H
#define UNIFORM_H

#include <core/shared_lib_class.h>
#include <core/types.h>
#include <core/buffer_layout.h>
#include <string>



namespace undicht {

    namespace graphics {

        SHARED_LIB_DECL_BASE_CLASS(Uniform, UniformBase, createUniform, copyUniform, deleteUniform);

        SHARED_LIB_CLASS(class Uniform : public UniformBase {
                /** uniforms are used to load variables to a shader
                * this includes simple types such as ints, floats, ... and buffers
                * the uniforms can be accessed in the shader via their names */
            public:
                // all data should be set before loading the uniform to a shader

                /** the uniform stores a single type,
                * see core/types.h for options
                * @param copy data: if true, the data gets copied into a pointer private to the uniform
                * if false, the given data pointer will be kept as the data storage (so dont delete it) */
                virtual void setData(const void* data, int type, bool copy_data = true);

                /** stores the data in a buffer accessable in the shader */
                virtual void setData(const void* data, core::BufferLayout layout, int num_of_elements);

                /** sets the name with which the uniforms data can be accessed in the shader */
                virtual void setName(const std::string& name);

            public:
                // functions to retrieve data

                virtual const std::string& getName() const;
                virtual int getType() const;
                virtual const void* getData() const;
                virtual const core::BufferLayout& getLayout() const;


                Uniform();
                virtual ~Uniform();

        };)

    } // graphics

} // undicht

#endif // UNIFORM_H
