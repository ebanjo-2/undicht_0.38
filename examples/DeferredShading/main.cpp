#include <iostream>
#include <window/window.h>
#include <core/graphics_core.h>
#include <core/renderer.h>
#include <deferred_shading/geometry_buffer.h>

#include <glm/gtc/type_ptr.hpp>

#include <files/model_loading/collada/collada_file.h>

#include <camera/perspective_camera_3d.h>
#include <key_input.h>
#include <mouse_input.h>

#include <time.h>

using namespace undicht;
using namespace graphics;
using namespace tools;
using namespace user_input;

// rendering
void initRenderer();

// drawing the scene data to a geometry buffer
Renderer* gbuffer_renderer = 0;
Shader* gbuffer_shader = 0;
GeometryBuffer* gbuffer = 0;

// drawing the final result to the screen
Renderer* final_renderer = 0;
Shader* final_shader = 0;
VertexBuffer* screen_quad = 0;

void updateViewPort(Window& window);
int width, height, old_width, old_height;

void endFrame(Window& window);

// scene data
void initScene();
std::vector<VertexBuffer*> vbos;
std::vector<Texture*> texs;
std::vector<int> tex_ids;

// camera control
KeyInput* key_input;
MouseInput* mouse_input;
float pitch = 0.0f, yaw = 0.0f;
void moveCam(Camera3D& cam);
void loadCam(Camera3D& cam, Shader shader);

// delete all objects
void terminate();

int main() {


	Window window(800, 600, "HELLO WORLD!");
	//window.setWindowMode(true, false);

	{

		initRenderer();
		initScene();

		key_input->setInputWindow(&window);
		mouse_input->setInputWindow(&window);

		PerspectiveCamera3D cam;
		cam.setPosition(glm::vec3(0, 0, -7));

		loadCam(cam, *gbuffer_shader);


		while (!window.shouldClose()) {

			final_renderer->clearFramebuffer(0.1, 0.1, 0.7, 1.0F);
			gbuffer_renderer->clearFramebuffer(0.6, 0.1, 0.1, 1.0f);

			// drawing the model to the geometry buffer
			for (int i = 0; i < vbos.size(); i++) {

				gbuffer_shader->loadTexture(*texs[tex_ids[i]]);
				gbuffer_renderer->submit(vbos[i]);
				gbuffer_renderer->draw();

			}

			// drawing the geometry buffer to the visible framebuffer

			final_shader->loadTexture(*gbuffer->getColorBuffer());
			final_shader->loadTexture(*gbuffer->getNormalBuffer());
			final_renderer->submit(screen_quad);
			final_renderer->draw();

			endFrame(window);

			moveCam(cam);
			loadCam(cam, *gbuffer_shader);
		}

		terminate();
		window.close();

	}

	return 0;
}


void initRenderer() {

	// Geometry Stage
	File gbuffer_shader_file("src/gbuffer_shader.glsl");
	std::string shader_src;
	gbuffer_shader_file.getAll(shader_src);

	gbuffer_shader = new Shader;
	gbuffer_shader->loadSource(shader_src);

	gbuffer = new GeometryBuffer;
	gbuffer->setSize(800, 600);
	gbuffer->setDepthBuffer();
	gbuffer->setColorBuffer();
	//gbuffer->setPositionBuffer();
	gbuffer->setNormalBuffer();

	gbuffer_renderer = new Renderer;
	gbuffer_renderer->submit(gbuffer_shader);
	gbuffer_renderer->submit(gbuffer);
	gbuffer_renderer->enableDepthTest(true);
	gbuffer_renderer->enableBackFaceCulling(true);

	// Final Stage
	File final_shader_file;
	shader_src.clear();
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


	key_input = new KeyInput;
	mouse_input = new MouseInput;
}

void updateViewPort(Window& window) {

	window.getSize(width, height);

	if ((width != old_width) || (height != old_height)) {

		final_renderer->setViewport(width, height);
		gbuffer_renderer->setViewport(width, height);
		gbuffer->setSize(width, height);
	}

	old_height = height;
	old_width = width;

}

void endFrame(Window& window) {

	std::cout << "FPS: " << getFPS() << "\n";

	key_input->clearKeyLists();
	mouse_input->clearButtonLists();
	mouse_input->updateCursorOffset();
	window.update();

	if (key_input->getKeyState(UND_KEY_E)) {

		window.setCursorVisible(!window.getCursorVisible());
	}

	updateViewPort(window);

}


void initScene() {

	initEngineTime();

	std::vector<MeshData> mesh_data;
	std::vector<TextureData> texture_data;

	ColladaFile model_loader;
	model_loader.open("res/sponza_collada/sponza.dae");
	//model_loader.open("res/SupertuxTex.dae");

	model_loader.loadAllMeshes(mesh_data);
	model_loader.loadAllTextures(texture_data);

	for (MeshData& md : mesh_data) {

		vbos.emplace_back(new VertexBuffer);
		vbos.back()->setLayout(md.vertex_layout);
		vbos.back()->setData(md.vertices);

		tex_ids.push_back(md.color_texture);
	}

	for (TextureData& td : texture_data) {

		texs.emplace_back(new Texture);
		texs.back()->setName("color"); // var name in shader

		if (td.pixel_layout.getTotalSize()) {
			// setting texture data

			texs.back()->setPixelFormat(td.pixel_layout);
			texs.back()->setSize(td.width, td.height, 1);
			texs.back()->setData(td.pixels.data(), td.pixels.size());

		} else {
			// setting blank texture data ("texture missing")

			unsigned char blank[3] = {255, 50, 100}; // pink

			texs.back()->setPixelFormat(BufferLayout({ UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR }));
			texs.back()->setSize(1, 1, 1);
			texs.back()->setData((char*)blank, 3 * sizeof(unsigned char));
		}

	}

}


void moveCam(Camera3D& cam) {

	if (key_input->getKeyState(UND_KEY_W)) {

		cam.addTranslation(10.0f * cam.getViewDirection());

	}

	if (key_input->getKeyState(UND_KEY_S)) {

		cam.addTranslation(-10.0f * cam.getViewDirection());
	}

	if (key_input->getKeyState(UND_KEY_A)) {

		cam.addTranslation(10.0f * cam.getRightDirection());
	}

	if (key_input->getKeyState(UND_KEY_D)) {

		cam.addTranslation(-10.0f * cam.getRightDirection());
	}
	
	float offsetx, offsety;

	mouse_input->getCursorOffset(offsetx, offsety);

	yaw += offsetx * 40;
	pitch -= offsety * 40;

	cam.setAxesRotation({0, pitch, yaw}, {UND_Z_AXIS, UND_X_AXIS, UND_Y_AXIS });

}


void loadCam(Camera3D& cam, Shader shader) {

	Uniform u;

	u.setName("proj");
	u.setData(glm::value_ptr(cam.getCameraProjectionMatrix()), UND_MAT4F);
	shader.loadUniform(u);

	u.setName("view");
	u.setData(glm::value_ptr(cam.getTransfMat()), UND_MAT4F);
	shader.loadUniform(u);

}

void terminate() {

	delete gbuffer;
	delete gbuffer_renderer;
	delete gbuffer_shader;

	delete final_renderer;
	delete final_shader;
	delete screen_quad;

	delete key_input;
	delete mouse_input;

	for (VertexBuffer* vbo : vbos) {
		delete vbo;
	}

	for (Texture* tex : texs) {
		delete tex;
	}
}