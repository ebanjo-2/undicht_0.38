#include <light_renderer.h>
#include <files/file.h>
#include <files/model_loading/collada/collada_file.h>


Renderer* light_renderer;
Shader* light_shader;
FrameBuffer* light_buffer;
Texture* light_map;
VertexBuffer* light_model;

Uniform* light_pos;
Uniform* light_color;
Uniform* light_radius;

Uniform* viewport_size;


void initLightRenderer() {

	File light_shader_file("src/light_shader.glsl");
	std::string shader_src;
	light_shader_file.getAll(shader_src);

	light_shader = new Shader;
	light_shader->loadSource(shader_src);

	light_map = new Texture;
	light_map->setPixelFormat(BufferLayout({ UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR }));
	light_map->setFilteringMethod(UND_NEAREST, UND_NEAREST);
	light_map->setWrappingMethod(UND_REPEAT);
	light_map->setName("light_map");

	light_buffer = new FrameBuffer;
	light_buffer->setSize(800, 600);
	light_buffer->addAttachment(*light_map, UND_COLOR_ATTACHMENT, 0);

	light_renderer = new Renderer;
	light_renderer->submit(light_shader);
	light_renderer->submit(light_buffer);
	light_renderer->enableDepthTest(false);
	light_renderer->enableBackFaceCulling(true, UND_FRONT_FACE);
	light_renderer->enableBlending(true, UND_SRC_ALPHA, UND_ONE_MINUS_SRC_ALPHA);

	light_model = new VertexBuffer;
	ColladaFile model_loader;
	model_loader.open("res/light.dae");
	MeshData data;
	model_loader.getMesh(data);

	light_model->setData(data.vertices);
	light_model->setLayout(data.vertex_layout);

	light_pos = new Uniform;
	light_pos->setName("light_origin");

	light_radius = new Uniform;
	light_radius->setName("light_radius");

	light_color = new Uniform;
	light_color->setName("light_color");

	viewport_size = new Uniform;
	viewport_size->setName("viewport_size");
}


void termLightRenderer() {

	delete light_renderer;
	delete light_shader;
	delete light_buffer;
	delete light_map;
	delete light_model;

	delete light_pos;
	delete light_color;
	delete light_radius;

	delete viewport_size;
}