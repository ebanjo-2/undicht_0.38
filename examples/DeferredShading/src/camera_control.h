#include <key_input.h>
#include <mouse_input.h>
#include <camera/perspective_camera_3d.h>
#include <core/shader.h>


using namespace undicht::user_input;
using namespace undicht::graphics;
using namespace undicht::tools;

extern KeyInput* key_input;
extern MouseInput* mouse_input;


extern float pitch;
extern float yaw;

void initCam();
void termCam();

void moveCam(PerspectiveCamera3D& cam);
void loadCam(PerspectiveCamera3D& cam, Shader shader);