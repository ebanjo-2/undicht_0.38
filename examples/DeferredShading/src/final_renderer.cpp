#include <final_renderer.h>
#include <files/file.h>

Renderer* final_renderer = 0;
Shader* final_shader = 0;
VertexBuffer* screen_quad = 0;

void initFinalRenderer() {

	File final_shader_file;
	std::string shader_src;
	final_shader_file.open("src/final_shader.glsl");
	final_shader = new Shader;
	final_shader->loadSource(final_shader_file.getAll(shader_src));

	final_renderer = new Renderer;
	final_renderer->submit(final_shader);
	final_renderer->enableDepthTest(false);
	final_renderer->enableBackFaceCulling(false);

	screen_quad = new VertexBuffer;
	screen_quad->setLayout(BufferLayout({ UND_VEC3F, UND_VEC2F }));

	// quad vertices taken from https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/8.2.deferred_shading_volumes/deferred_shading_volumes.cpp
	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
	};

	unsigned int quadIndices[] = {
		0,1,2,
		1,3,2
	};

	screen_quad->setData(quadVertices, sizeof(quadVertices));
	screen_quad->setIndexData(quadIndices, sizeof(quadIndices));
}


void termFinalRenderer() {

	delete final_renderer;
	delete final_shader;
	delete screen_quad;
}