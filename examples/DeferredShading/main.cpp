#include <iostream>
#include <window/window.h>

#include <time.h>

#include <camera_control.h>
#include <scene.h>

#include <deferred_shading/gbuffer_renderer.h>
#include <deferred_shading/light_renderer.h>
#include <deferred_shading/final_renderer.h>

#include <glm/gtc/type_ptr.hpp>


using namespace undicht;
using namespace graphics;
using namespace tools;
using namespace user_input;

// viewport data
int width, height, old_width, old_height;

void endFrame(Window& window);



std::array<Light, 10> lights = {

	Light(glm::vec3(0,0,0), glm::vec3(0.5,1.0,0.5), 400),
	Light(glm::vec3(0,0,0), glm::vec3(0.2,0.2,1.0), 150),
	Light(glm::vec3(0,0,0), glm::vec3(1.0,0.2,1.0), 150),
	Light(glm::vec3(0,0,0), glm::vec3(0.2,1.0,0.2), 150),
	Light(glm::vec3(0,0,0), glm::vec3(1.0,0.2,1.0), 150),
	Light(glm::vec3(0,0,0), glm::vec3(0.2,0.2,1.0), 150),
	Light(glm::vec3(0,0,0), glm::vec3(1.0,0.2,1.0), 150),
	Light(glm::vec3(0,0,0), glm::vec3(0.2,1.0,0.2), 150),
	Light(glm::vec3(0,0,0), glm::vec3(1.0,0.2,1.0), 150),
	Light(glm::vec3(0,0,0), glm::vec3(0.2,0.2,1.0), 150),

};

double last_time;

void updateLights() {

	if ((getEngineTime() - last_time) > 0.3) {

		for (int i = 8; i >= 0; i--) {

			lights[i + 1].m_position = lights[i].m_position;
		}


		last_time = getEngineTime();
	}

}

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

		GBufferRenderer gbuffer_renderer;
		LightRenderer light_renderer;
		FinalRenderer final_renderer;


		while (!window.shouldClose()) {

			final_renderer.clearFramebuffer(0.1, 0.1, 0.1, 1.0F);
			light_renderer.clearFramebuffer(0.0, 0.0, 0.0, 0.0f);
			gbuffer_renderer.clearFramebuffer(0.6, 0.1, 0.6, 1.0f);


			// drawing the model to the geometry buffer
			 for(int i = 0; i < vbos.size(); i++) {

				gbuffer_renderer.m_shader.loadTexture(*texs[tex_ids[i]]);
				gbuffer_renderer.submit(vbos[i]);
				gbuffer_renderer.draw();

			}

			// drawing the lights to the light map
			 light_renderer.submitGBuffer(gbuffer_renderer.getGBuffer());

			for (const Light& light : lights) {

				light_renderer.drawLight(light);
			}

			// drawing the geometry buffer to the visible framebuffer

			final_renderer.submitGBuffer(gbuffer_renderer.getGBuffer());
			final_renderer.submitLightMap(light_renderer.getLightMap());
			final_renderer.drawFinal();


			endFrame(window);

			// updating the viewport in case the window was resized 
			window.getSize(width, height);

			if ((width != old_width) || (height != old_height)) {

				final_renderer.setViewport(width, height);
				gbuffer_renderer.setViewport(width, height);
				light_renderer.setViewport(width, height);

			}

			old_height = height;
			old_width = width;


			// moving the camera + lights
			moveCam(cam);

			lights[0].m_position = cam.getPosition();
			updateLights();

			gbuffer_renderer.loadCam(cam);
			light_renderer.loadCam(cam);

		}

		terminate();
		window.close();

	}

	return 0;
}


void endFrame(Window& window) {

	// std::cout << "FPS: " << getFPS() << "\n";

	key_input->clearKeyLists();
	mouse_input->clearButtonLists();
	mouse_input->updateCursorOffset();
	window.update();

	if (key_input->getKeyState(UND_KEY_E)) {

		window.setCursorVisible(!window.getCursorVisible());
	}

}


void initialize() {

	initEngineTime();
	initCam();
	initScene();

}

void terminate() {

	termCam();
	termScene();

}