#include <iostream>


#include <window/window.h>
#include <vector_graphics/vg_renderer.h>
#include <mouse_input.h>


using namespace undicht;
using namespace graphics;
using namespace user_input;


int main() {

    Window window(1000, 1000, "Vector Graphics + Noise");

    {

        VGRenderer r;
        r.setViewport(1000, 1000);
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
            r.drawLine(glm::vec2(0.2, 0.5), glm::vec2( -0.1,-0.4), 0.10f);

            window.update();
        }


    }


    return 0;
}
