#include <gbuffer_renderer.h>
#include <files/file.h>


Renderer* gbuffer_renderer = 0;
Shader* gbuffer_shader = 0;
GeometryBuffer* gbuffer = 0;


void initGBufferRenderer() {

	File gbuffer_shader_file("src/gbuffer_shader.glsl");
	std::string shader_src;
	gbuffer_shader_file.getAll(shader_src);

	gbuffer_shader = new Shader;
	gbuffer_shader->loadSource(shader_src);

	gbuffer = new GeometryBuffer;
	gbuffer->setSize(800, 600);
	gbuffer->setDepthBuffer();
	gbuffer->setColorBuffer();
	gbuffer->setPositionBuffer();
	gbuffer->setNormalBuffer();

	gbuffer_renderer = new Renderer;
	gbuffer_renderer->submit(gbuffer_shader);
	gbuffer_renderer->submit(gbuffer);
	gbuffer_renderer->enableDepthTest(true);
	gbuffer_renderer->enableBackFaceCulling(true);

}


void termGBufferRenderer() {

	delete gbuffer;
	delete gbuffer_renderer;
	delete gbuffer_shader;
}