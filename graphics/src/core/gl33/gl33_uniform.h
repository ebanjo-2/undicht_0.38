#ifndef GL33_UNIFORM_H
#define GL33_UNIFORM_H

#include <graphics/uniform.h>
#include <string>
#include <memory>
#include <core/types.h>
#include <core/shared_id.h>


namespace undicht {

    namespace graphics {

        namespace gl33 {


            class Uniform : public implementation::Uniform {

                public:
                    // members

                    core::SharedID m_ubo_id = 0; // only used if the uniform stores its data in a buffer

                    std::string m_name;
                    int m_type = core::UND_TYPE_UNAVAILABLE;
                    core::BufferLayout m_layout; // used only if the uniform stores a buffer
                    core::Shared<char*> m_data = 0;
                    bool m_data_copied = false;


                public:

                    /** the uniform stores a single type,
                    * see core/types.h for options */
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



            };

        } // gl33

    } // graphics

} // undicht

#endif // GL33_UNIFORM_H
