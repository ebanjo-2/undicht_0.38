#include <iostream>


#include <window/window.h>
#include <vector_graphics/vg_renderer.h>
#include <mouse_input.h>


using namespace undicht;
using namespace graphics;
using namespace user_input;


int main() {

    //const int WINDOW_WIDTH = 1680;
    //const int WINDOW_HEIGHT = 1050;
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    const float ASPECT_RATIO = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Vector Graphics + Noise");

    {

        VGRenderer r;
        r.setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);
        MouseInput m;
        m.setInputWindow(&window);

        while(!window.shouldClose()) {

            r.clearFramebuffer(0,0,.25,0);

            float x, y;
            m.getCursorPosition(x,y);

            r.setColor(glm::vec4(1.,1.,1.,0.5));

            r.drawLine(glm::vec2(-0.2,-0.3), glm::vec2(-0.2, 0.0), 0.01f);
            r.drawLine(glm::vec2(-0.2, 0.0), glm::vec2( 0.2, 0.0), 0.01f);
            r.drawLine(glm::vec2( 0.2, 0.0), glm::vec2( 0.2,-0.4), 0.10f);
            r.drawLine(glm::vec2(-0.2,-0.3), glm::vec2( 0.2,-0.4), 0.03f);

            r.setColor(glm::vec4(0.,1.,0.,0.5));
            r.drawLine(glm::vec2(-0.2, 0.0), glm::vec2( 0.2,-0.4), 0.10f);

            r.setColor(glm::vec4(1.,0.,0.,0.5));
            r.drawLine(glm::vec2(x * ASPECT_RATIO, y), glm::vec2( -0.1,-0.4), 0.10f);

            r.setColor(glm::vec4(1.,0.5,0.3,0.7));
            r.drawCircle(glm::vec2(0.0, 0.0), 1.0, 0.05f);

            window.update();
        }


    }


    return 0;
}
