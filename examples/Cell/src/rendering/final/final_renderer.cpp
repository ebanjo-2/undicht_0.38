#include "final_renderer.h"
#include <files/file.h>

#include <string_tools.h>

#include <types.h>


#define FINAL_CELL_SHADER getFilePath(UND_CODE_SRC_FILE) + "final_cell_shader.glsl"

// https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/4.1.textures/textures.cpp
#define SCREEN_QUAD_VERTICES {1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f}
#define SCREEN_QUAD_INDICES  {0, 1, 3, 1, 2, 3}
#define SCREEN_QUAD_LAYOUT {UND_VEC2F}


using namespace undicht;
using namespace graphics;
using namespace tools;

namespace cell {

	FinalRenderer::FinalRenderer() {

		// init shader
		File file(FINAL_CELL_SHADER);
		std::string shader_src;
		file.getAll(shader_src);

		m_shader.loadSource(shader_src);

		// init screen quad
		m_screen_quad.setData(SCREEN_QUAD_VERTICES);
		m_screen_quad.setIndexData(SCREEN_QUAD_INDICES);
		m_screen_quad.setLayout(BufferLayout(SCREEN_QUAD_LAYOUT));

		// renderer setting
		enableBackFaceCulling(false);
		enableDepthTest(false);


	}


	FinalRenderer::~FinalRenderer() {

	}

	void FinalRenderer::drawFinal(const CellGBuffer& scene_cell_data, const MaterialRegister& materials) {

		m_shader.loadTexture(materials.getTextureAtlas());

		m_shader.loadTexture(scene_cell_data.m_uvs);
		m_shader.loadTexture(scene_cell_data.m_material);
		m_shader.loadTexture(scene_cell_data.m_normals);

		submit(&m_shader);
		submit(&m_screen_quad);

		draw();

	}


} // cell