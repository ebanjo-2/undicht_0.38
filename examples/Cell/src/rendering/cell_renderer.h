#ifndef CELL_RENDERER_H
#define CELL_RENDERER_H


#include <core/renderer.h>

#include <core/shader.h>

#include <camera/perspective_camera_3d.h>

#include <cell.h>
#include <chunk.h>
#include <material.h>

#include "chunk_buffer.h"

#include <files/model_loading/image_loader.h>
#include <core/texture.h>

#define CR_VIEW_RANGE 3
#define CR_VIEW_RANGE_CUBED 27


class CellRenderer : public undicht::graphics::Renderer {

public:
	// storing the data of registered materials

	struct RegisteredMaterial {

		std::string name;
		int prefix;

	};

	static std::vector<RegisteredMaterial> s_materials;
	static std::vector<std::string> s_prefixes;
	static undicht::graphics::Texture* s_texture_atlas; // contains the texture data of all materials


public:

	// camera uniforms
	static undicht::graphics::Uniform* s_proj;
	static undicht::graphics::Uniform* s_view;

	// chunk uniforms
    static undicht::graphics::Uniform* s_chunk_offset;

	static undicht::graphics::Shader* s_shader;

	static int s_renderer_count;

	static void init();
	static void term();

public:
    // the data that gets drawn by the renderer

    std::array<ChunkBuffer<8>, CR_VIEW_RANGE_CUBED> m_chunk_buffers;
    std::array<unsigned int, CR_VIEW_RANGE_CUBED> m_cells_per_chunk;

    /** (0,0,0) is the center chunk, x,y and z are offsets from there */
    void updateChunk(const CompactChunk<8>& chunk_data, int x, int y, int z);

    int getChunkID(int x, int y, int z);

public:
	// managing materials

	static Material registerMaterial(const std::string& prefix, const std::string& name);

	static void setMaterialTexture(const Material& mat, undicht::tools::TextureData& data);

public:
	// drawing

	void loadCam(undicht::graphics::PerspectiveCamera3D& cam);

	void drawChunks();

	CellRenderer();
	virtual ~CellRenderer();

};

#endif // CELL_RENDERER_H
