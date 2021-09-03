#include <stdio.h>

#include <iostream>

#include <font_rendering/font.h>
#include <font_rendering/font_renderer.h>
#include <window/window.h>

#include <string_tools.h>

#include <undicht_time.h>

#include <player/player.h>
#include <rendering/cell_renderer.h>
#include <rendering/texture_atlas.h>


#include <world/cell.h>
#include <math/cell_math.h>

using namespace undicht;
using namespace graphics;
using namespace tools;
using namespace user_input;

using namespace cell;

void drawCrosshair(FontRenderer& fr, Font& f);

int main(int argc, char **argv) {

    const int WINDOW_WIDTH = 1600;
    const int WINDOW_HEIGHT = 900;

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "HELLO WORLD");
    window.setCursorVisible(false);

    initEngineTime();

    {

        Font arial("res/FreeMono.ttf", 250);

        KeyInput key_input;
        key_input.setInputWindow(&window);

        MouseInput mouse_input;
        mouse_input.setInputWindow(&window);

        Player player(glm::vec3(0,0,0));

        CellRenderer renderer;
        FontRenderer font_renderer;

        renderer.setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);
        font_renderer.setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);

        TextureAtlas texture_atlas;
        int dirt = texture_atlas.addTexture("res/dirt.png");
        int grass = texture_atlas.addTexture("res/grass.png");
        int iron_ore = texture_atlas.addTexture("res/iron_ore.png");
        int wood_floor = texture_atlas.addTexture("res/wood_floor.png");

        std::cout << "dirt id: " << dirt << "\n";

        Chunk chunk_0;
        chunk_0.m_cells.emplace_back(Cell(u8vec3(0,10,0), u8vec3(10,20,4)));

        double last_time;

        Cell c0(u8vec3(0,20,0), u8vec3(20, 0, 20));
        Cell c1(u8vec3(5,5,0), u8vec3(15, 15, 20));

        std::array<Cell, 6> diff;
        subtractCells(c0, c1, diff);

        std::cout << c0 << "\n";
        std::cout << "overlapping: " << overlappingVolume(c0, c1) << "\n";
        std::cout << getVolume(diff[0]) << "\n";
        std::cout << getVolume(diff[1]) << "\n";
        std::cout << getVolume(diff[2]) << "\n";
        std::cout << getVolume(diff[3]) << "\n";
        std::cout << getVolume(diff[4]) << "\n";
        std::cout << getVolume(diff[5]) << "\n";

        chunk_0.initEditCells();
        chunk_0.termEditCells();

        while(!window.shouldClose()) {

            renderer.clearFramebuffer();
            font_renderer.clearFramebuffer();

            player.loadKeyInput(key_input);
            player.loadMouseInput(mouse_input);

            renderer.loadCam(player);
            renderer.loadTextureAtlas(texture_atlas);

            renderer.draw(glm::vec3(0,0,-5));
            renderer.draw(glm::vec3(1,1,0));
            renderer.draw(glm::vec3(-1,1,0));
            renderer.draw(glm::vec3(-1,5,0));
            renderer.draw(glm::vec3(5,2,0));

            renderer.draw(chunk_0, glm::vec3(0,0,0));

            font_renderer.setFontColor(glm::vec3(0.8f, 0.8f, 0.8f));

            drawCrosshair(font_renderer, arial);

            font_renderer.draw(arial, "FPS: " + toStr(1 / (getEngineTime() - last_time)), glm::vec2(-1.0f,0.8f));
            last_time = getEngineTime();


            key_input.clearKeyLists();
            mouse_input.updateCursorOffset();
            mouse_input.clearButtonLists();
            window.update();


        }

    }

    return 0;
}



void drawCrosshair(FontRenderer& fr, Font& f) {

    const float crosshair_size = 30;

    std::string crosshair = "+";

    glm::vec2 cr_rel_size = fr.getRelSize(f, crosshair, crosshair_size);

    glm::vec2 pos = glm::vec2(-0.5f, 2.0f/3.0f * -0.5f) * cr_rel_size;

    fr.draw(f, crosshair, pos, crosshair_size);




}
