#include "cell_renderer.h"
#include <files/file.h>
#include <string_tools.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <iostream>

using namespace undicht;
using namespace graphics;
using namespace tools;

#define CELL_SHADER getFilePath(UND_CODE_SRC_FILE) + "cell_shader.glsl"

// cube mesh build with help from here:
// https://pastebin.com/DXKEmvap

// starting with the 4 vertices on the back
#define CUBE_POSITIONS {0,0,0, 1,0,0, 1,1,0, 0,1,0, 0,0,1, 1,0,1, 1,1,1, 0,1,1}
#define CUBE_UVS {0,0, 1,0, 1,1, 0,1}
#define CUBE_NORMALS {0,0,1, 1,0,0, 0,0,-1, -1,0,0, 0,1,0, 0,-1,0}

//#define CUBE_INDICES {0, 1, 3, 3, 1, 2, 1, 5, 2, 2, 5, 6, 5, 4, 6, 6, 4, 7, 4, 0, 7, 7, 0, 3, 3, 2, 7, 7, 2, 6, 4, 5, 0, 0, 5, 1}
#define CUBE_INDICES {0, 3, 1, 3, 2, 1, 1, 2, 5, 2, 6, 5, 5, 6, 4, 6, 7, 4, 4, 7, 0, 7, 3, 0, 3, 7, 2, 7, 6, 2, 4, 0, 5, 0, 1, 5}
//#define CUBE_UV_INDICES {0, 1, 3, 3, 1, 2}
#define CUBE_UV_INDICES {0, 3, 1, 3, 2, 1}

// the buffer that stores the cells of a chunk
VertexBuffer* CellRenderer::s_buffer;

// camera uniforms
Uniform* CellRenderer::s_proj;
Uniform* CellRenderer::s_view;

Shader* CellRenderer::s_shader;

int CellRenderer::s_renderer_count = 0;


std::vector<CellRenderer::RegisteredMaterial> CellRenderer::s_materials;

std::vector<std::string> CellRenderer::s_prefixes;

Texture* CellRenderer::s_texture_atlas;

void CellRenderer::init() {


	s_buffer = new VertexBuffer;
	s_proj = new Uniform;
	s_view = new Uniform;
	s_shader = new Shader;

	// loading the shader src file
	File cell_shader_file(CELL_SHADER);
	std::string shader_src;
	cell_shader_file.getAll(shader_src);

	s_shader->loadSource(shader_src);

	// camera uniforms
	s_proj->setName("proj");
	s_view->setName("view");

	// building the vertices of a cube

	{

		float positions[] = CUBE_POSITIONS;
		float uvs[] = CUBE_UVS;
		float normals[] = CUBE_NORMALS;

		int indices[] = CUBE_INDICES;
		int uv_indices[] = CUBE_UV_INDICES;

		// there are 6 faces, so 12 triangles with 3 vertices each and 8 floats per vertex
		std::array<float, 12 * 3 * 8> vertices;

		for (int i = 0; i < 12 * 3; i++) { // going through every vertex of the cube

			vertices[i * 8 + 0] = positions[indices[i] * 3 + 0];
			vertices[i * 8 + 1] = positions[indices[i] * 3 + 1];
			vertices[i * 8 + 2] = positions[indices[i] * 3 + 2];

			vertices[i * 8 + 3] = uvs[uv_indices[(i % 6)] * 2 + 0];
			vertices[i * 8 + 4] = uvs[uv_indices[(i % 6)] * 2 + 1];

			vertices[i * 8 + 5] = normals[indices[i / 6] * 3 + 0];
			vertices[i * 8 + 6] = normals[indices[i / 6] * 3 + 1];
			vertices[i * 8 + 7] = normals[indices[i / 6] * 3 + 2];
		}

		BufferLayout layout({ UND_VEC3F, UND_VEC2F, UND_VEC3F });
		
		s_buffer->setLayout(layout);
		s_buffer->setData(vertices.data(), sizeof(vertices));

	}

	// initializing the material texture atlas

	{

		s_texture_atlas = new Texture;
		s_texture_atlas->setName("texture_atlas");
		s_texture_atlas->setSize(16, 16, 16); // 16 materials is the limit for now

		BufferLayout layout({ UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR });
		s_texture_atlas->setPixelFormat(layout);

	}


}


void CellRenderer::term() {

	delete s_buffer;
	delete s_proj;
	delete s_view;
	delete s_shader;

	delete s_texture_atlas;
}

CellRenderer::CellRenderer() {

	if (!s_renderer_count) init();

	submit(s_shader);
	submit(s_buffer);
	enableDepthTest(true);
	enableBackFaceCulling(true);


	s_renderer_count++;

}

CellRenderer::~CellRenderer() {

	s_renderer_count--;

	if(!s_renderer_count)
		term();
}


///////////////////////////////////////// managing materials /////////////////////////////////////////

Material CellRenderer::registerMaterial(const std::string& prefix, const std::string& name) {

	RegisteredMaterial reg_mat;
	reg_mat.name = name;

	// searching for the prefix
	for (int i = 0; i < s_prefixes.size(); i++) {

		if (!prefix.compare(s_prefixes.at(i))) {

			reg_mat.prefix = i;
			break;
		}

	}

	Material new_mat;
	new_mat.setName(prefix, name);
	new_mat.setID(s_materials.size());

	s_materials.push_back(reg_mat);

	return new_mat;
}

void CellRenderer::setMaterialTexture(const Material& mat, TextureData& data) {

	if (mat.m_id > 15) {

		std::cout << "ERROR: maximum material count is 16" << "\n";
		return;
	}

	s_texture_atlas->setData(data.pixels.data(), sizeof(data.pixels), mat.m_id);

}


////////////////////////////////////////////// drawing ///////////////////////////////////////////////

void CellRenderer::loadCam(undicht::graphics::PerspectiveCamera3D& cam) {

	// loading camera uniforms
	s_proj->setData(glm::value_ptr(cam.getCameraProjectionMatrix()), UND_MAT4F);
	s_shader->loadUniform(*s_proj);

	s_view->setData(glm::value_ptr(cam.getViewMatrix()), UND_MAT4F);
	s_shader->loadUniform(*s_view);
}

void CellRenderer::drawChunk(const CompactChunk<4>& c) {

	// storing the cellchunks in the vertex buffers per instance data

	std::vector<int> cell_data;

	for (const CellChunk<4>& cc : c.m_cells) {

		cell_data.push_back(cc.m_pos_x);
		cell_data.push_back(cc.m_pos_y);
		cell_data.push_back(cc.m_pos_z);

		cell_data.push_back(cc.m_size_x + 1);
		cell_data.push_back(cc.m_size_y + 1);
		cell_data.push_back(cc.m_size_z + 1);

		cell_data.push_back(cc.m_material);
	}

	BufferLayout layout({ UND_VEC3I, UND_VEC3I, UND_INT });

	s_buffer->setInstanceLayout(layout);
	s_buffer->setInstanceData(cell_data.data(), cell_data.size() * sizeof(int));

	s_shader->loadTexture(*s_texture_atlas);

	submit(s_buffer);

	draw(c.m_cells.size());

}

