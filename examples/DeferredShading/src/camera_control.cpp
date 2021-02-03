#include <camera_control.h>
#include <glm/gtc/type_ptr.hpp>
#include <types.h>

KeyInput* key_input;
MouseInput* mouse_input;

float pitch = 0.0f;
float yaw = 0.0f;

void initCam() {

	key_input = new KeyInput;
	mouse_input = new MouseInput;
}


void termCam() {

	delete key_input;
	delete mouse_input;
}

void moveCam(PerspectiveCamera3D& cam) {

	if (key_input->getKeyState(UND_KEY_W)) {

		cam.addTranslation(-10.0f * cam.getViewDirection());

	}

	if (key_input->getKeyState(UND_KEY_S)) {

		cam.addTranslation(10.0f * cam.getViewDirection());
	}

	if (key_input->getKeyState(UND_KEY_A)) {

		cam.addTranslation(-10.0f * cam.getRightDirection());
	}

	if (key_input->getKeyState(UND_KEY_D)) {

		cam.addTranslation(10.0f * cam.getRightDirection());
	}

	float offsetx, offsety;

	mouse_input->getCursorOffset(offsetx, offsety);

	yaw -= offsetx * 40;
	pitch += offsety * 40;

	cam.setAxesRotation({ 0, pitch, yaw }, { UND_Z_AXIS, UND_X_AXIS, UND_Y_AXIS });

}


void loadCam(PerspectiveCamera3D& cam, Shader shader) {

	Uniform u;

	u.setName("proj");
	u.setData(glm::value_ptr(cam.getCameraProjectionMatrix()), UND_MAT4F);
	shader.loadUniform(u);

	u.setName("view");
	u.setData(glm::value_ptr(cam.getViewMatrix()), UND_MAT4F);
	shader.loadUniform(u);

	u.setName("cam_pos");
	u.setData(glm::value_ptr(cam.getWorldPosition()), UND_VEC3F);
	shader.loadUniform(u);

	u.setName("viewrange");
	float viewrange = cam.getViewRange();
	u.setData(&viewrange, UND_FLOAT);
	shader.loadUniform(u);

	float dvbvr = 1 / (cam.getViewRange() * 2); // 1 divided by double the viewrange (used to convert position data into something that can be stored in a texture)
	u.setName("dvbvr");
	u.setData(&dvbvr, UND_FLOAT);
	shader.loadUniform(u);

}