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

    const int WINDOW_WIDTH = 1650;
    const int WINDOW_HEIGHT = 1050;

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "HELLO WORLD");
    window.setCursorVisible(false);

    initEngineTime();

    {

        Font arial("res/FreeMono.ttf", 25);

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

        u8vec3 v1(10, 0, 20);

        Chunk chunk_0;

        chunk_0.setCells({
            Cell(u8vec3(0,0,0), u8vec3(255,20,255), 0),
            Cell(u8vec3(5,5,5), u8vec3(10,10,10), 0),
            Cell(u8vec3(9,5,5), u8vec3(15,10,10), 1),
            Cell(u8vec3(6,2,6), u8vec3(7,5,7), 1),
        });


        double last_time;

        //chunk_0.initDrawBuffer();

        while(!window.shouldClose()) {

            renderer.clearFramebuffer();
            font_renderer.clearFramebuffer();

            player.loadKeyInput(key_input);
            player.loadMouseInput(mouse_input);

            if(key_input.getKeyState(UND_KEY_E))
                chunk_0.setCell(Cell(u8vec3(player.getPosition() - glm::vec3(10, 10, 10)), u8vec3(player.getPosition() + glm::vec3(10, 10, 10)), -1));

            renderer.loadCam(player);
            renderer.loadTextureAtlas(texture_atlas);
            renderer.draw(chunk_0, glm::vec3(0,0,0));

            font_renderer.setFontColor(glm::vec3(0.8f, 0.8f, 0.8f));
            drawCrosshair(font_renderer, arial);
            font_renderer.draw(arial, "FPS: " + toStr(1 / (getEngineTime() - last_time)), glm::vec2(-1.0f,0.8f));
            font_renderer.draw(arial, "Drawn Cells: " + toStr(chunk_0.getCellCount()), glm::vec2(-1.0f,0.7f));
            font_renderer.draw(arial, "Player Position: " + toStr(player.getPosition().x) + " " + toStr(player.getPosition().y) + " " + toStr(player.getPosition().z), glm::vec2(-1.0f,0.6f));
            font_renderer.draw(arial, "Unused Cells: " + toStr(chunk_0.m_unused_cells.size()), glm::vec2(-1.0f,0.5f));
            font_renderer.draw(arial, "Valid Chunk: " + toStr(chunk_0.validVolume() ? "true" : "false"), glm::vec2(-1.0f,0.4f));


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
