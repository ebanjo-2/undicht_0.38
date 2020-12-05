#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <core/graphics_core.h>

#include <buffer_layout.h>
#include <vector>

namespace undicht {

    namespace graphics {

		namespace interf {

			class VertexBuffer {
				/** a class that can hold multiple vertices which can be used to build meshes */
			public:
				// vertex data

				/** stores the content of data in the vertex buffer
				* @param offset: (in bytes) at what point in the buffer the data should be stored (0: first byte of first vertex) */
				virtual void setData(const std::vector<float>& data, unsigned int offset = 0) = 0;

				/** @param size: (in bytes) the size of the data to copy from the data buffer */
				virtual void setData(const void* data, unsigned int size, unsigned int offset = 0) = 0;


				/** retrieve the data from the vertex buffer and store it in the vector
				* @param num_float: how many floats to copy from the buffer (not vertices !)
				* @param offset: (in bytes) at what point in the buffer to start copying */
				virtual void getData(std::vector<float>& data, unsigned int num_floats, unsigned int offset = 0) = 0;

				/** data should have at least #size bytes reserved */
				virtual void getData(void* data, unsigned int size, unsigned int offset = 0) = 0;
			public:
				// additional vertex related data

				/** determines how many and what types of components belong to a vertex
				* also determines how the data can be accessed in a vertex shader
				* example for a vertex: 3D position, 2D texture coordinate, 3D normal
				* translates into layout: UND_VEC3F, UND_VEC2F, UND_VEC3F */
				virtual void setLayout(const tools::BufferLayout& layout) = 0;

				/** returns the number of bytes stored in the buffer */
				virtual unsigned int getSize() = 0;

				/** returns the size of one vertex */
				virtual unsigned int getVertexSize() = 0;

				/** returns the layout of the stored vertices */
				virtual const tools::BufferLayout& getLayout() = 0;


			public:
				// indices

				/** @brief an index array determines the order in which vertices are read from the vertex buffer
				* (if no index array is set the order is 0,1,2,3, ...)
				* this allows redundant vertices to be removed and the now unique vertices to be reused
				* (may only be useful on models with a lot of redundant vertices)
				* setting index data will also enable the usage of that data
				* @param offset: (in bytes) the position to copy the data to */
				virtual void setIndexData(const std::vector<int>& data, unsigned int offset = 0) = 0;

				/** @param size: the size (in bytes) of index data to copy from the data buffer */
				virtual void setIndexData(const void* data, unsigned int size, unsigned int offset = 0) = 0;

				/** @brief retrieve data from the index array
				* @param num_indices: number of indices to copy
				* @param offset: (in bytes) position in the index array to copy from */
				virtual void getIndexData(std::vector<int>& data, unsigned int num_indices, unsigned int offset = 0) = 0;

				/** @param data should have at least #size bytes reserved
				* @param the number of bytes to copy (number of indices * sizeof(int)) */
				virtual void getIndexData(void* data, unsigned int size, unsigned int offset = 0) = 0;

			public:
				// additional index related data

				/** @return the number of bytes stored in the indices buffer */
				virtual unsigned int getIndexBufferSize() = 0;

				virtual void setUsesIndices(bool use_indices = true) = 0;
				virtual bool getUsesIndices() = 0;


				VertexBuffer() = default;
				virtual ~VertexBuffer() = default;

			};

		} // interf

    } // graphics

} // undicht


#ifdef USE_GL33

#include "core/gl33/gl33_vertex_buffer.h"

namespace undicht {
	namespace graphics {
		typedef gl33::VertexBuffer VertexBuffer;
	} // graphics
} // undicht

#endif // USE_GL33

#endif // VERTEX_BUFFER_H
