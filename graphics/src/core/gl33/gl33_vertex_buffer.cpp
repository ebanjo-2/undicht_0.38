#include "core/graphics_core.h"

#ifdef USE_GL33

#include <iostream>

#include <glad/glad.h>

#include "core/vertex_buffer.h"
#include "event_logger.h"
#include "gl33_tools.h"

// decided not to use the good old abstraction (core) from the previous attempts of undicht, but to write opengl code directly
// into the implementation of the api

using namespace undicht::tools;

namespace undicht {

	namespace graphics {

		namespace gl33 {

			unsigned int bound_vao = 0;


			VertexBuffer::VertexBuffer() {
				//ctor

				unsigned int temp_vbo;
				unsigned int temp_ibo;
				unsigned int temp_ebo;
				unsigned int temp_vao;

				glGenBuffers(1, &temp_vbo);
				glGenBuffers(1, &temp_ibo);
				glGenBuffers(1, &temp_ebo);
				glGenVertexArrays(1, &temp_vao);

				m_vbo_id.setID(temp_vbo);
				m_ibo_id.setID(temp_ibo);
				m_ebo_id.setID(temp_ebo);
				m_vao_id.setID(temp_vao);

				bind();
			}

			VertexBuffer::~VertexBuffer() {

				if (m_vbo_id.removeUser()) {
					glDeleteBuffers(1, &m_vbo_id.getID());
				}

				if (m_ibo_id.removeUser()) {
					glDeleteBuffers(1, &m_ibo_id.getID());
				}

				if (m_ebo_id.removeUser()) {
					glDeleteBuffers(1, &m_ebo_id.getID());
				}

				if (m_vao_id.removeUser()) {
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
				if (offset + size > getSize()) {
					resize(offset + size);
				}

				bind();
				glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
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



			void VertexBuffer::setLayout(const tools::BufferLayout& layout) {
				/** determines how many and what types of components belong to a vertex
				* also determines how the data can be accessed in a vertex shader
				* example for a vertex: 3D position, 2D texture coordinate, 3D normal
				* translates into layout: UND_VEC3F, UND_VEC2F, UND_VEC3F */

				m_layout = layout;

				bind();

				// telling opengl about the layout via attribute pointers
				setAttrPointer(layout, m_vbo_id, getVertexSize());

				// in case the instance layout was set before the vertex layout
				// to make sure that the instance attributes follow the vertex attributes
				setInstanceLayout(m_instance_layout);

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


			const tools::BufferLayout& VertexBuffer::getLayout() {
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

				if (offset + size > getIndexBufferSize()) {

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

			///////////////////////////////////////////// per instance data ///////////////////////////////////////////////

			/** when using instanced rendering, you may use this buffer to store data
			* that changes from instance to instance but not between the vertices
			* this data can be accessed in the shader just as vertex attributes,
			* their indexes follow the vertex attributes */

			void VertexBuffer::setInstanceData(const std::vector<float>& data, unsigned int offset) {
				/* @param offset : (in bytes) at what point in the buffer the data should be stored(0: first byte) */

				setInstanceData(data.data(), data.size() * sizeof(float), offset);
			}

			void VertexBuffer::setInstanceData(const void* data, unsigned int size, unsigned int offset) {
				/** @param size: (in bytes) the size of the data to copy from the data buffer */

				// testing if resize is needed
				if (offset + size > getInstanceBufferSize()) {
					resizeInstanceBuffer(offset + size);
				}

				bind();
				glBindBuffer(GL_ARRAY_BUFFER, m_ibo_id);
				glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);

				UND_CHECK_GL_ERROR();

			}

			void VertexBuffer::getInstanceData(std::vector<float>& data, unsigned int num_floats, unsigned int offset) {
				/** retrieve the data from the instance buffer and store it in the vector
				* @param num_float: how many floats to copy from the buffer
				* @param offset: (in bytes) at what point in the buffer to start copying */

				// to be done
			}

			void VertexBuffer::getInstanceData(void* data, unsigned int size, unsigned int offset) {
				/** data should have at least #size bytes reserved */

				// to be done
			}


			///////////////////////////////////////////// additional instance related data /////////////////////////////////////////////


			void VertexBuffer::setInstanceLayout(const tools::BufferLayout& layout) {
				/** determines how many and what types of components belong to the per instance data
				* also determines how the data can be accessed in a vertex shader
				* example for a vertex: 3D position, 2D texture coordinate, 3D normal
				* translates into layout: UND_VEC3F, UND_VEC2F, UND_VEC3F */

				m_instance_layout = layout;

				// the per instance attributes come after the vertex attributes
				int per_vertex_attr = m_layout.m_types.size();

				// telling opengl about the layout via attribute pointers
				setAttrPointer(layout, m_ibo_id, getInstanceSize(), per_vertex_attr);

				bind();
				glBindBuffer(GL_ARRAY_BUFFER, m_ibo_id);

				// telling opengl that the attributes are per instance, not vertex
				for (int i = 0; i < layout.m_types.size(); i++) {

					glVertexAttribDivisor(i + m_layout.m_types.size(), 1);
				}

				UND_CHECK_GL_ERROR();

			}


			unsigned int VertexBuffer::getInstanceBufferSize() {
				/** returns the number of bytes stored in the instance buffer */

				return m_instance_size;
			}


			unsigned int VertexBuffer::getInstanceSize() {
				/** returns the size of the data for one instance */

				return m_instance_layout.getTotalSize();
			}


			const tools::BufferLayout& VertexBuffer::getInstanceLayout() {
				/** returns the layout of the stored data for each instance */

				return m_instance_layout;
			}

			///////////////////////////////////////////// non api functions /////////////////////////////////////////////



			void VertexBuffer::setAttrPointer(const tools::BufferLayout& layout, int buffer_id, int buffer_chunk_size, unsigned int attr_index_offset) {
				/** tells the vao how to construct vertex data using the given buffers
				* determines how many and what types of components belong to a vertex
				* also determines how the data can be accessed in a vertex shader
				* example for a vertex: 3D position, 2D texture coordinate, 3D normal
				* translates into layout: UND_VEC3F, UND_VEC2F, UND_VEC3F
				* @param buffer_id: the gl buffer from which to take the data
				* @param attr_index_offset: used when the attr pointers for per instance data are set  */

				bind();
				glBindBuffer(GL_ARRAY_BUFFER, buffer_id);

				int attr_id = 0;
				int attr_sum = 0;

				for (const int& attr : layout.m_types) {
					if (attr == UND_FLOAT) {

						glVertexAttribPointer(attr_index_offset + attr_id, 1, GL_FLOAT, GL_FALSE, buffer_chunk_size, (void*)attr_sum); // will always produce a warning
						glEnableVertexAttribArray(attr_index_offset + attr_id);
						attr_sum += sizeof(float);
					}
					else if (attr == UND_INT) {

						glVertexAttribPointer(attr_index_offset + attr_id, 1, GL_INT, GL_FALSE, buffer_chunk_size, (void*)attr_sum); // will always produce a warning
						glEnableVertexAttribArray(attr_index_offset + attr_id);
						attr_sum += sizeof(int);
					} else if (attr == UND_UINT8) {

                        glVertexAttribPointer(attr_index_offset + attr_id, 1,  GL_UNSIGNED_BYTE, GL_FALSE, buffer_chunk_size, (void*)attr_sum); // will always produce a warning
						glEnableVertexAttribArray(attr_index_offset + attr_id);
						attr_sum += sizeof(unsigned char);
					}
					else if (attr == UND_VEC3F) {

						/*std::cout << "setting vec3 attribute, id= " << attr_index_offset + attr_id << "\n";
						std::cout << "buffer_chunk_size " << buffer_chunk_size << "\n";
						std::cout << "attr_sum " << attr_sum << "\n";*/

						glVertexAttribPointer(attr_index_offset + attr_id, 3, GL_FLOAT, GL_FALSE, buffer_chunk_size, (void*)attr_sum); // will always produce a warning
						glEnableVertexAttribArray(attr_index_offset + attr_id);
						attr_sum += 3 * sizeof(float);
					}
					else if (attr == UND_VEC2F) {

						//std::cout << "setting vec2 attribute, id= " << attr_id << "\n";

						glVertexAttribPointer(attr_index_offset + attr_id, 2, GL_FLOAT, GL_FALSE, buffer_chunk_size, (void*)attr_sum); // will always produce a warning
						glEnableVertexAttribArray(attr_index_offset + attr_id);
						attr_sum += 2 * sizeof(float);
					}
					else if (attr == UND_VEC3I) {

						glVertexAttribPointer(attr_index_offset + attr_id, 3, GL_INT, GL_FALSE, buffer_chunk_size, (void*)attr_sum); // will always produce a warning
						glEnableVertexAttribArray(attr_index_offset + attr_id);
						attr_sum += 3 * sizeof(int);
					}
					else if (attr == UND_MAT4F) {
						glVertexAttribPointer(attr_index_offset + attr_id, 16, GL_FLOAT, GL_FALSE, buffer_chunk_size, (void*)attr_sum); // will always produce a warning
						glEnableVertexAttribArray(attr_index_offset + attr_id);
						attr_sum += 16 * sizeof(float);
					}

					attr_id++;
				}

			}

			void VertexBuffer::resize(unsigned int size) { // taken and modified from the old opengl abstraction
				/** resizes the buffer, old data gets copied into the new buffer
				* (if the buffers size is decreased the data is cut of at the end) */


				// saving the data
				unsigned char* old_data = new unsigned char[getSize()];
				unsigned int old_data_size = getSize();

				getData(old_data, getSize(), 0);

				// resizing the buffer and restoring the data
				if (getSize() < size) {
					// increasing the size of the buffer (old_data not big enough)

					bind();
					glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);

					glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
					glBufferSubData(GL_ARRAY_BUFFER, 0, old_data_size, old_data);

				}
				else {
					// decreasing the size of the buffer (old_data big enough)

					bind();
					glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);

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
				if (getIndexBufferSize() < size) {
					// increasing the size of the buffer (old_data not big enough)

					bind();
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
					glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, old_data_size, old_data);

				}
				else {
					// decreasing the size of the buffer (old_data big enough)

					bind();
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, old_data, GL_STATIC_DRAW);
				}

				m_indices_size = size;
				delete[] old_data; // temporary buffer

				UND_CHECK_GL_ERROR();
			}

			void VertexBuffer::resizeInstanceBuffer(unsigned int size) {

				// saving the data
				unsigned char* old_data = new unsigned char[getInstanceBufferSize()];
				unsigned int old_data_size = getInstanceBufferSize();

				getInstanceData(old_data, getInstanceBufferSize(), 0);


				// resizing the buffer and restoring the data
				if (getInstanceBufferSize() < size) {
					// increasing the size of the buffer (old_data not big enough)

					bind();
					glBindBuffer(GL_ARRAY_BUFFER, m_ibo_id);

					glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
					glBufferSubData(GL_ARRAY_BUFFER, 0, old_data_size, old_data);

				} else {
					// decreasing the size of the buffer (old_data big enough)

					bind();
					glBindBuffer(GL_ARRAY_BUFFER, m_ibo_id);

					glBufferData(GL_ARRAY_BUFFER, size, old_data, GL_STATIC_DRAW);
				}

				m_instance_size = size;
				delete[] old_data; // temporary buffer

				UND_CHECK_GL_ERROR();

			}


			void VertexBuffer::bind() {
				/** binds the vao, vbo and ebo */

				if (bound_vao != m_vao_id) {
					// binding can take quite some time, so checking if its redundant may improve performance

					glBindVertexArray(m_vao_id);
					glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
					glBindBuffer(GL_ARRAY_BUFFER, m_ibo_id);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_id);
					bound_vao = m_vao_id;

				}

			}


		} // gl33

	} // graphics

} // undicht

#endif // USE_GL33
