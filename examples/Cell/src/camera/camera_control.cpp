#include "camera_control.h"
#include <glm/gtc/type_ptr.hpp>
#include <types.h>

#include <key_input.h>
#include <mouse_input.h>
#include <camera/perspective_camera_3d.h>

#include <iostream>


using namespace undicht::user_input;
using namespace undicht::graphics;
using namespace undicht::tools;

KeyInput* key_input;
MouseInput* mouse_input;

float pitch = 0.0f;
float yaw = 0.0f;

float speed = 1;

void initCam() {

	key_input = new KeyInput;
	mouse_input = new MouseInput;
}


void termCam() {

	delete key_input;
	delete mouse_input;
}

void moveCam(PerspectiveCamera3D& cam) {

	if (key_input->getKeyState(UND_KEY_LSHIFT)) {

		speed = 0.1;
	} else {

		speed = 0.9; // speed and power
	}

	if (key_input->getKeyState(UND_KEY_W)) {

		cam.addTranslation(-1 * speed * cam.getViewDirection());

	}

	if (key_input->getKeyState(UND_KEY_S)) {

		cam.addTranslation(1 * speed  * cam.getViewDirection());
	}

	if (key_input->getKeyState(UND_KEY_A)) {

		cam.addTranslation(-1 * speed * cam.getRightDirection());
	}

	if (key_input->getKeyState(UND_KEY_D)) {

		cam.addTranslation(1 * speed  * cam.getRightDirection());
	}

	float offsetx, offsety;

	mouse_input->getCursorOffset(offsetx, offsety);

	yaw -= offsetx * 40;
	pitch += offsety * 40;

	cam.setAxesRotation({ 0, pitch, yaw }, { UND_Z_AXIS, UND_X_AXIS, UND_Y_AXIS });

}
