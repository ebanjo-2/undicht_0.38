#include <stdio.h>

#include <iostream>

#include <font_rendering/font.h>
#include <font_rendering/font_renderer.h>
#include <window/window.h>

#include <string_tools.h>

#include <undicht_time.h>

#include <player/player.h>
#include <rendering/cell_renderer.h>

using namespace undicht;
using namespace graphics;
using namespace tools;
using namespace user_input;

using namespace cell;

int main(int argc, char **argv) {

    Window window(1650, 1050, "HELLO WORLD");
    window.setCursorVisible(false);

    initEngineTime();

    {

        Font arial("res/arial.ttf", 50);

        KeyInput key_input;
        key_input.setInputWindow(&window);

        MouseInput mouse_input;
        mouse_input.setInputWindow(&window);

        Player player(glm::vec3(0,0,0));

        CellRenderer renderer;
        FontRenderer font_renderer;

        renderer.setViewport(1650, 1050);
        font_renderer.setViewport(1650, 1050);

        double last_time;

        while(!window.shouldClose()) {

            renderer.clearFramebuffer();
            font_renderer.clearFramebuffer();

            player.loadKeyInput(key_input);
            player.loadMouseInput(mouse_input);

            renderer.loadCam(player);
            renderer.draw(glm::vec3(0,0,0));
            renderer.draw(glm::vec3(1,1,0));
            renderer.draw(glm::vec3(-1,1,0));
            renderer.draw(glm::vec3(0,2,0));

            font_renderer.setFontColor(glm::vec3(0.8f, 0.8f, 0.8f));
            font_renderer.draw(arial, "+", glm::vec2(0,0), 30);

            font_renderer.draw(arial, "FPS: " + toStr(1 / (getEngineTime() - last_time)), glm::vec2(-1,0.8));
            last_time = getEngineTime();


            key_input.clearKeyLists();
            mouse_input.updateCursorOffset();
            mouse_input.clearButtonLists();
            window.update();


        }

    }

    return 0;
}
