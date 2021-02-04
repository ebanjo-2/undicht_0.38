#ifndef GEOMETRY_BUFFER_H
#define GEOMETRY_BUFFER_H

#include <core/frame_buffer.h>
#include <core/texture.h>

#include <array>

namespace undicht {

	namespace graphics {


		class GeometryBuffer : public FrameBuffer {
			/** a framebuffer that stores screen space geometry information 
			* like the color, position and normals of fragments */

		private:
			// dont copy GeometryBuffers
			// since the Framebuffer class stores a pointer to its attached textures, copying the 

			void operator=(const GeometryBuffer& b) {};
			GeometryBuffer(const GeometryBuffer& b) {};

		protected:

			// whether or not these buffers are set
			bool m_depth_buffer = false;
			bool m_color_buffer = false;
			bool m_position_buffer = false;
			bool m_normal_buffer = false;

			std::array<Texture, 4> m_buffers;

		public:
			// managing the buffers used to store the geometry data

			// the buffers can be written to by a fragment shader 
			// their adress is defined by the order in which they were set (first -> address 0)

			void setDepthBuffer(bool use_depth = true);

			void setColorBuffer(bool use_color = true);

			void setPositionBuffer(bool use_position = true);

			void setNormalBuffer(bool use_normal = true);

			const Texture* getDepthBuffer() const;

			const Texture* getColorBuffer() const;

			const Texture* getPositionBuffer() const;

			const Texture* getNormalBuffer() const;
		public:

			GeometryBuffer();
			virtual ~GeometryBuffer();

		};


	} // graphics

} // undicht

#endif // ! GEOMETRY_BUFFER_H