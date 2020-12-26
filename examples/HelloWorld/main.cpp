#include <iostream>
#include <window/window.h>
#include <core/graphics_core.h>
#include <core/renderer.h>

#include <glm/gtc/type_ptr.hpp>

#include <files/model_loading/collada/collada_file.h>

#include <camera/perspective_camera_3d.h>

using namespace undicht;
using namespace graphics;
using namespace tools;

// rendering
void initRenderer();
Renderer* renderer = 0;
Shader* shader = 0;

// scene data
void initScene();
VertexBuffer* vbo = 0;
Texture* tex = 0;

// camera control
void moveCam(Camera3D& cam);
void loadCam(Camera3D& cam, Shader shader);

// delete all objects
void terminate();

int main() {
	

	Window window(800, 600, "HELLO WORLD!");

	{

		initRenderer();
		initScene();

		PerspectiveCamera3D cam;
		cam.setPosition(glm::vec3(0, 0, -7));

		loadCam(cam, *shader);

		while (!window.shouldClose()) {

			renderer->clearFramebuffer(0.1, 0.6, 0.1, 1.0F);

			shader->loadTexture(*tex);
			renderer->submit(vbo);
			renderer->draw();

			window.update();
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

}

void initScene() {

	MeshData mesh_data;
	TextureData texture_data;

	ColladaFile model_loader;
	model_loader.open("res/SupertuxTex.dae");
	model_loader.getMesh(mesh_data);
	model_loader.getTexture(texture_data, mesh_data.color_texture);

	vbo = new VertexBuffer;
	vbo->setLayout(mesh_data.vertex_layout);
	vbo->setData(mesh_data.vertices);

	tex = new Texture;
	tex->setName("color");
	tex->setPixelFormat(texture_data.pixel_layout);
	tex->setSize(texture_data.width, texture_data.height, 1);
	tex->setData(texture_data.pixels.data(), texture_data.pixels.size());
}


void moveCam(Camera3D& cam) {

	cam.setPosition(glm::vec3(0, 0, 0));

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

	delete vbo;
	delete tex;
}