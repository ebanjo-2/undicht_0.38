#include <iostream>
#include <window/window.h>
#include <core/graphics_core.h>
#include <core/renderer.h>
#include <core/frame_buffer.h>

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
Renderer* renderer = 0;
Shader* shader = 0;

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

	{

		initRenderer();
		initScene();

		key_input->setInputWindow(&window);
		mouse_input->setInputWindow(&window);

		PerspectiveCamera3D cam;
		cam.setPosition(glm::vec3(0, 0, -7));

		loadCam(cam, *shader);


		while (!window.shouldClose()) {

			renderer->clearFramebuffer(0.1, 0.6, 0.1, 1.0F);

			for (int i = 0; i < vbos.size(); i++) {

				shader->loadTexture(*texs[tex_ids[i]]);
				renderer->submit(vbos[i]);
				renderer->draw();

			}

			endFrame(window);

			moveCam(cam);
			loadCam(cam, *shader);
		}

		terminate();
		window.close();

	}

	return 0;
}


void initRenderer() {

	File shader_file("res/shader.glsl");
	std::string shader_src;
	shader_file.getAll(shader_src);

	shader = new Shader;
	shader->loadSource(shader_src);

	renderer = new Renderer;
	renderer->submit(shader);
	renderer->enableDepthTest(true);
	renderer->enableBackFaceCulling(false);

	key_input = new KeyInput;
	mouse_input = new MouseInput;
}

void updateViewPort(Window& window) {

	window.getSize(width, height);

	if ((width != old_width) || (height != old_height)) {

		renderer->setViewport(width, height);
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

	delete renderer;
	delete shader;

	delete key_input;
	delete mouse_input;

	for (VertexBuffer* vbo : vbos) {
		delete vbo;
	}

	for (Texture* tex : texs) {
		delete tex;
	}
}