#include "cell_gbuffer.h"

#include <types.h>

using namespace undicht;
using namespace graphics;
using namespace tools;

namespace cell {


	CellGBuffer::CellGBuffer() {
		
		init();
	}

	CellGBuffer::CellGBuffer(unsigned int width, unsigned int height) {

		setSize(width, height);
		init();
	}

	CellGBuffer::~CellGBuffer() {


	}

	void CellGBuffer::init() {

		// depth map
		m_depth_map.setName("depth_map");
		addAttachment(m_depth_map, UND_DEPTH_ATTACHMENT_READ_AND_WRITE);

		// uv texture
		m_uvs.setName("uvs");
		m_uvs.setPixelFormat(BufferLayout({ UND_UINT8, UND_UINT8 })); // 2 channels with one byte each
		m_uvs.setFilteringMethod(UND_NEAREST, UND_NEAREST);
		m_uvs.setWrappingMethod(UND_REPEAT);
		addAttachment(m_uvs, UND_COLOR_ATTACHMENT, 0);

		// material texture
		m_material.setName("material");
		m_material.setPixelFormat(BufferLayout({ UND_UINT16 })); // 1 channel to store the ushort
		m_material.setFilteringMethod(UND_NEAREST, UND_NEAREST);
		m_material.setWrappingMethod(UND_REPEAT);
		addAttachment(m_material, UND_COLOR_ATTACHMENT, 1);

		// normal texture
		m_normals.setName("normal");
		m_normals.setPixelFormat(BufferLayout({ UND_UINT2, UND_UINT2, UND_UINT2, UND_UINT2})); // 3 channels needed for the normal, 1 channel used to fill up to 1 byte per pixel
		m_normals.setFilteringMethod(UND_NEAREST, UND_NEAREST);
		m_normals.setWrappingMethod(UND_REPEAT);
		addAttachment(m_normals, UND_COLOR_ATTACHMENT, 2);

		// 2bytes + 2 bytes + 1 byte = 5 bytes per fragment (+ depth map)
	}

} // cell