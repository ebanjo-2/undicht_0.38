#include <iostream>
#include <window/window.h>

#include <time.h>

#include <camera_control.h>
#include <scene.h>

#include <gbuffer_renderer.h>
#include <light_renderer.h>
#include <final_renderer.h>

#include <glm/gtc/type_ptr.hpp>


using namespace undicht;
using namespace graphics;
using namespace tools;
using namespace user_input;


void updateViewPort(Window& window);
int width, height, old_width, old_height;

void endFrame(Window& window);

class Light {

public:

	float m_radius;

	glm::vec3 m_color;
	glm::vec3 m_position;

public:

	Light(const glm::vec3& pos, const glm::vec3& color, float radius)
		: m_radius(radius), m_color(color), m_position(pos) {};

	virtual ~Light() {};


};


const std::array<Light, 16> lights = {

	Light(glm::vec3(300,100,-160), glm::vec3(0.2,0.2,1.0), 150),
	Light(glm::vec3(300,100,160), glm::vec3(1.0,0.2,1.0), 150),
	Light(glm::vec3(0,100,-160), glm::vec3(0.2,1.0,0.2), 150),
	Light(glm::vec3(0,100,160), glm::vec3(1.0,0.2,1.0), 150),
	Light(glm::vec3(-300,100,-160), glm::vec3(0.2,0.2,1.0), 150),
	Light(glm::vec3(-300,100,160), glm::vec3(1.0,0.2,1.0), 150),
	Light(glm::vec3(-600,100,-160), glm::vec3(0.2,1.0,0.2), 150),
	Light(glm::vec3(-600,100,160), glm::vec3(1.0,0.2,1.0), 150),
	Light(glm::vec3(300,500,-160), glm::vec3(0.2,0.2,1.0), 150),
	Light(glm::vec3(300,500,160), glm::vec3(1.0,0.2,1.0), 150),
	Light(glm::vec3(0,500,-160), glm::vec3(0.2,1.0,0.2), 150),
	Light(glm::vec3(0,500,160), glm::vec3(0.0,0.2,1.0), 150),
	Light(glm::vec3(-300,500,-160), glm::vec3(0.2,0.2,1.0), 150),
	Light(glm::vec3(-300,500,160), glm::vec3(0.0,0.2,1.0), 150),
	Light(glm::vec3(-600,500,-160), glm::vec3(0.2,1.0,0.2), 150),
	Light(glm::vec3(-600,500,160), glm::vec3(0.0,0.2,1.0), 150)

};

// initialize all objects
void initialize();

// delete all objects
void terminate();

int main() {


	Window window(800, 600, "HELLO WORLD!");
	//window.setWindowMode(true, false);

	{

		initialize();

		key_input->setInputWindow(&window);
		mouse_input->setInputWindow(&window);

		PerspectiveCamera3D cam;
		cam.setViewRange(0.1, 10000);
		//cam.setPosition(glm::vec3(0, 0, -7));

		loadCam(cam, *gbuffer_shader);
		loadCam(cam, *light_shader);


		while (!window.shouldClose()) {

			final_renderer->clearFramebuffer(0.1, 0.1, 0.1, 1.0F);
			light_renderer->clearFramebuffer(0.0, 0.0, 0.0, 0.0f);
			gbuffer_renderer->clearFramebuffer(0.6, 0.1, 0.6, 1.0f);


			// drawing the model to the geometry buffer
			 for(int i = 0; i < vbos.size(); i++) {

				gbuffer_shader->loadTexture(*texs[tex_ids[i]]);
				gbuffer_renderer->submit(vbos[i]);
				gbuffer_renderer->draw();

			}

			// drawing the lights to the light map
			light_shader->loadTexture(*gbuffer->getPositionBuffer());
			light_shader->loadTexture(*gbuffer->getNormalBuffer());

			light_renderer->submit(light_model);

			for (const Light& light : lights) {

				light_pos->setData(glm::value_ptr(light.m_position), UND_VEC3F);
				light_shader->loadUniform(*light_pos);

				light_color->setData(glm::value_ptr(light.m_color), UND_VEC3F);
				light_shader->loadUniform(*light_color);

				light_radius->setData(&light.m_radius, UND_FLOAT);
				light_shader->loadUniform(*light_radius);

				light_renderer->draw();
			}

			// drawing the geometry buffer to the visible framebuffer

			final_shader->loadTexture(*gbuffer->getColorBuffer());
			final_shader->loadTexture(*light_map);

			final_renderer->submit(screen_quad);
			final_renderer->draw();

			endFrame(window);

			moveCam(cam);
			loadCam(cam, *gbuffer_shader);
			loadCam(cam, *light_shader);
		}

		terminate();
		window.close();

	}

	return 0;
}

void updateViewPort(Window& window) {

	window.getSize(width, height);

	if ((width != old_width) || (height != old_height)) {

		final_renderer->setViewport(width, height);
		gbuffer_renderer->setViewport(width, height);
		gbuffer->setSize(width, height);
		light_renderer->setViewport(width, height);
		light_buffer->setSize(width, height);

		float viewport[] = { 1.0f / width, 1.0f / height };
		viewport_size->setData(viewport, UND_VEC2F);
		light_shader->loadUniform(*viewport_size);
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


void initialize() {

	initEngineTime();
	initCam();
	initScene();

	initGBufferRenderer();
	initLightRenderer();
	initFinalRenderer();

}

void terminate() {

	termCam();
	termScene();

	termGBufferRenderer();
	termLightRenderer();
	termFinalRenderer();

}