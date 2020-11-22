#include <glad/glad.h>

#include "gl33_vertex_buffer.h"
#include <core/shared_lib.h>
#include <core/event_logger.h>
#include "gl_tools.h"
#include <iostream>

// decided not to use the good old abstraction (core) from the previous attempts of undicht, but to write opengl code directly
// into the implementation of the api

using namespace undicht::core;


namespace undicht {

    namespace graphics {

        namespace gl33 {

            unsigned int bound_vao = 0;


            VertexBuffer::VertexBuffer() {
                //ctor

                unsigned int temp_vbo;
                unsigned int temp_ebo;
                unsigned int temp_vao;

                glGenBuffers(1, &temp_vbo);
                glGenBuffers(1, &temp_ebo);
                glGenVertexArrays(1, &temp_vao);

                m_vbo_id.setID(temp_vbo);
                m_ebo_id.setID(temp_ebo);
                m_vao_id.setID(temp_vao);

                bind();
            }

            VertexBuffer::~VertexBuffer() {

                if(m_vbo_id.removeUser()) {
                    glDeleteBuffers(1, &m_vbo_id.getID());
                }

                if(m_ebo_id.removeUser()) {
                    glDeleteBuffers(1, &m_ebo_id.getID());
                }

                if(m_vao_id.removeUser()) {
                    glDeleteVertexArrays(1, &m_vao_id.getID());
                }

            }

            ////////////////////////////////////////// vertex data //////////////////////////////////////////


            void VertexBuffer::setData(const std::vector<float>& data, unsigned int offset) {
                /** stores the content of data in the vertex buffer
                * @param offset: (in bytes) at what point in the buffer the data should be stored (0: first byte of first vertex) */

                setData(data.data(), data.size() * sizeof(float), offset);

            }

            void VertexBuffer::setData(const void* data, unsigned int size, unsigned int offset) {
                /** @param size: (in bytes) the size of the data to copy from the data buffer */

                // testing if resize is needed
                if(offset + size > getSize()) {
                    resize(offset + size);
                }

                bind();
                glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);

                UND_CHECK_GL_ERROR();

            }

            void VertexBuffer::getData(std::vector<float>& data, unsigned int num_floats, unsigned int offset) {
                /** retrieve the data from the vertex buffer and store it in the vector
                * @param num_float: how many floats to copy from the buffer (not vertices !)
                * @param offset: (in bytes) at what point in the buffer to start copying */
                // to be done
                UND_CHECK_GL_ERROR();
            }

            void VertexBuffer::getData(void* data, unsigned int size, unsigned int offset) {
                /** data should have at least #size bytes reserved */
                // to be done
                UND_CHECK_GL_ERROR();
            }

            /////////////////////////////////////// additional vertex related data ////////////////////////////////////////////



            void VertexBuffer::setLayout(const core::BufferLayout& layout) {
                /** determines how many and what types of components belong to a vertex
                * also determines how the data can be accessed in a vertex shader
                * example for a vertex: 3D position, 2D texture coordinate, 3D normal
                * translates into layout: UND_VEC3F, UND_VEC2F, UND_VEC3F */

                m_layout = layout;

                // telling opengl about the layout via attribute pointers
                int vertex_size = getVertexSize();
                int attr_id = 0;
                int attr_sum = 0;

                bind();


                for(const int& attr : layout.m_types) {
                    if(attr == UND_FLOAT) {

                        glVertexAttribPointer(attr_id, 1, GL_FLOAT, GL_FALSE, vertex_size, (void*)attr_sum); // will always produce a warning
                        glEnableVertexAttribArray(attr_id);
                        attr_sum += sizeof(float);
                    } else if( attr == UND_INT) {

                        glVertexAttribPointer(attr_id, 1, GL_INT, GL_FALSE, vertex_size, (void*)attr_sum); // will always produce a warning
                        glEnableVertexAttribArray(attr_id);
                        attr_sum += sizeof(int);
                    } else if( attr == UND_VEC3F) {

                        //std::cout << "setting vec3 attribute, id= " << attr_id << "\n";

                        glVertexAttribPointer(attr_id, 3, GL_FLOAT, GL_FALSE, vertex_size, (void*)attr_sum); // will always produce a warning
                        glEnableVertexAttribArray(attr_id);
                        attr_sum += 3 * sizeof(float);
                    } else if( attr == UND_VEC2F) {

                        //std::cout << "setting vec2 attribute, id= " << attr_id << "\n";

                        glVertexAttribPointer(attr_id, 2, GL_FLOAT, GL_FALSE, vertex_size, (void*)attr_sum); // will always produce a warning
                        glEnableVertexAttribArray(attr_id);
                        attr_sum += 2 * sizeof(float);
                    } else if( attr == UND_MAT4F) {
                        glVertexAttribPointer(attr_id, 16, GL_FLOAT, GL_FALSE, vertex_size, (void*)attr_sum); // will always produce a warning
                        glEnableVertexAttribArray(attr_id);
                        attr_sum += 16 * sizeof(float);
                    }

                    attr_id++;
                }

                UND_CHECK_GL_ERROR();
            }


            unsigned int VertexBuffer::getSize() {
                /** returns the number of bytes stored in the buffer */

                return m_size;
            }

            unsigned int VertexBuffer::getVertexSize() {
                /** returns the size of one vertex */

                return m_layout.getTotalSize();
            }


            const core::BufferLayout& VertexBuffer::getLayout() {
                /** returns the layout of the stored vertices */

                return m_layout;
            }

            ////////////////////////////////////////////////// indices //////////////////////////////////////////////////


            void VertexBuffer::setIndexData(const std::vector<int>& data, unsigned int offset) {
                /** @brief an index array determines the order in which vertices are read from the vertex buffer
                * (if no index array is set the order is 0,1,2,3, ...)
                * this allows redundant vertices to be removed and the now unique vertices to be reused
                * (may only be useful on models with a lot of redundant vertices)
                * setting index data will also enable the usage of that data
                * @param offset: (in bytes) the position to copy the data to */

                setIndexData(data.data(), data.size() * sizeof(int), offset);
            }


            void VertexBuffer::setIndexData(const void* data, unsigned int size, unsigned int offset) {
                /** @param size: the size (in bytes) of index data to copy from the data buffer */

                if(offset + size > getIndexBufferSize()) {

                    resizeIndexBuffer(offset + size);
                }

                bind();
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);

                setUsesIndices(true);

                UND_CHECK_GL_ERROR();
            }

            void VertexBuffer::getIndexData(std::vector<int>& data, unsigned int num_indices, unsigned int offset) {
                /** @brief retrieve data from the index array
                * @param num_indices: number of indices to copy
                * @param offset: (in bytes) position in the index array to copy from */

                // to be implemented
            }

            void VertexBuffer::getIndexData(void* data, unsigned int size, unsigned int offset) {
                /** @param data should have at least #size bytes reserved
                * @param the number of bytes to copy (number of indices * sizeof(int)) */

                // to be implemented
            }


            /////////////////////////////////////// additional index related data ///////////////////////////////////////

            unsigned int VertexBuffer::getIndexBufferSize() {
                /** @return the number of bytes stored in the indices buffer */

                return m_indices_size;
            }


            void VertexBuffer::setUsesIndices(bool use_indices) {

                m_uses_indices = use_indices;
            }


            bool VertexBuffer::getUsesIndices() {

                return m_uses_indices;
            }

            ///////////////////////////////////////////// non api functions /////////////////////////////////////////////

            void VertexBuffer::resize(unsigned int size) { // taken and modified from the old opengl abstraction
                /** resizes the buffer, old data gets copied into the new buffer
                * (if the buffers size is decreased the data is cut of at the end) */


                // saving the data
                unsigned char* old_data = new unsigned char[getSize()];
                unsigned int old_data_size = getSize();

                getData(old_data, getSize(), 0 );

                // resizing the buffer and restoring the data
                if(getSize() < size) {
                    // increasing the size of the buffer (old_data not big enough)

                    bind();
                    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, old_data_size, old_data);

                } else {
                    // decreasing the size of the buffer (old_data big enough)

                    bind();
                    glBufferData(GL_ARRAY_BUFFER, size, old_data, GL_STATIC_DRAW);
                }

                m_size = size;
                delete[] old_data; // temporary buffer

                UND_CHECK_GL_ERROR();
            }

            void VertexBuffer::resizeIndexBuffer(unsigned int size) { // taken and modified from the old opengl abstraction
                /** resizes the buffer, old data gets copied into the new buffer
                * (if the buffers size is decreased the data is cut of at the end) */


                // saving the data
                unsigned char* old_data = new unsigned char[getIndexBufferSize()];
                unsigned int old_data_size = getIndexBufferSize();

                getIndexData(old_data, getIndexBufferSize(), 0);

                // resizing the buffer and restoring the data
                if(getIndexBufferSize() < size) {
                    // increasing the size of the buffer (old_data not big enough)

                    bind();
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
                    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, old_data_size, old_data);

                } else {
                    // decreasing the size of the buffer (old_data big enough)

                    bind();
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, old_data, GL_STATIC_DRAW);
                }

                m_indices_size = size;
                delete[] old_data; // temporary buffer

                UND_CHECK_GL_ERROR();
            }

            void VertexBuffer::bind() {
                /** binds the vao, vbo and ebo */

                if(bound_vao != m_vao_id) {
                    // binding can take quite some time, so checking if its redundant may improve performance

                    glBindVertexArray(m_vao_id);
                    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_id);
                    bound_vao = m_vao_id;

                }

            }



            //////////////////////////////// functions to load a VertexBuffer object from the shared lib  ////////////////////////////////////

            SHARED_LIB_EXPORT implementation::VertexBuffer* createVertexBuffer() {

                return new gl33::VertexBuffer;
            }


            SHARED_LIB_EXPORT void copyVertexBuffer(implementation::VertexBuffer* c, implementation::VertexBuffer* o) {

                VertexBuffer* c_vertex_buffer = (VertexBuffer*)c;

                if(c_vertex_buffer->m_vbo_id.removeUser()) {
                    glDeleteBuffers(1, &c_vertex_buffer->m_vbo_id.getID());
                }

                if(c_vertex_buffer->m_ebo_id.removeUser()) {
                    glDeleteBuffers(1, &c_vertex_buffer->m_ebo_id.getID());
                }

                if(c_vertex_buffer->m_vao_id.removeUser()) {
                    glDeleteVertexArrays(1, &c_vertex_buffer->m_vao_id.getID());
                }

                *c_vertex_buffer = *(VertexBuffer*)o;
            }

            SHARED_LIB_EXPORT void deleteVertexBuffer(implementation::VertexBuffer* vertex_buffer) {

                delete (VertexBuffer*)vertex_buffer;
            }



        } // gl33

    } // graphics

} //undicht
