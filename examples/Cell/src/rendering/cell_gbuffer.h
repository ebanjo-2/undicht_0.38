#ifndef CELL_GBUFFER_H
#define CELL_GBUFFER_H

#include <core/frame_buffer.h>

namespace cell {

	class CellGBuffer : public undicht::graphics::FrameBuffer {
		/** storing the cell data of the visible scene*/
	public:

		undicht::graphics::Texture m_depth_map; // depth map that can also be used to retrieve fragment position

		undicht::graphics::Texture m_uvs; // 2 channel texture containing one byte of uv data each (enough for 256 * 256 textures)
		undicht::graphics::Texture m_material; // 2 byte material id
		undicht::graphics::Texture m_normals; // 2 bits per channel are enough, since cells cant be rotated (2 bits will be left unused)

		void init();

	public:

		CellGBuffer(); // will initialize the textures + framebuffer
		CellGBuffer(unsigned int width, unsigned int height); // will initialize the textures + framebuffer
		virtual ~CellGBuffer();


	};

} // cell


#endif // CELL_GBUFFER_H
