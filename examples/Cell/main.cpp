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
#include <world/chunk_optimization.h>
#include <world/world.h>
#include <math/cell_math.h>

#include <undicht_thread.h>

#include <worldgen/world_file.h>

using namespace undicht;
using namespace graphics;
using namespace tools;
using namespace user_input;

using namespace cell;

void drawCrosshair(FontRenderer& fr, Font& f);
void drawInfo(FontRenderer& fr, Font& f, double last_time, Player& p, int cell_count);


int main(int argc, char **argv) {

    //const int WINDOW_WIDTH = 512;
    //const int WINDOW_HEIGHT = 300;
    const int WINDOW_WIDTH = 1680;
    const int WINDOW_HEIGHT = 1050;
    //const int WINDOW_WIDTH = 1920;
    //const int WINDOW_HEIGHT = 1080;

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "HELLO WORLD");
    //window.setWindowMode(true,false);
    window.setCursorVisible(false);

    initEngineTime();

    {
        KeyInput key_input;
        key_input.setInputWindow(&window);

        MouseInput mouse_input;
        mouse_input.setInputWindow(&window);

        Font arial("res/FreeMono.ttf", 25);

        std::cout << "if this isnt 8 bytes ... :" << sizeof(WorldFile::Header) << "\n";

        CellRenderer renderer;
        FontRenderer font_renderer;
        renderer.setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);
        font_renderer.setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);

        World world_0;

        TextureAtlas texture_atlas;
        int dirt = texture_atlas.addTexture("res/dirt32.png");
        int grass = texture_atlas.addTexture("res/Gras 1.0.png");
        int stone = texture_atlas.addTexture("res/stone32.png");
        int stone_wall = texture_atlas.addTexture("res/Backsteine2.png");
        int default_wood = texture_atlas.addTexture("res/default_wood.png");
        int holz_bretter = texture_atlas.addTexture("res/Holzbrettertextur.png");
        int holz = texture_atlas.addTexture("res/Holztextur.png");

        texture_atlas.m_tex.generateMipMaps();


        Player player(glm::vec3(0,25,0));

        double last_time = 0;


        std::vector<glm::vec3> lights;
        lights.push_back(glm::vec3(0,21,0));

        while(!(window.shouldClose() || key_input.getKeyState(UND_KEY_ESC))) {

            renderer.clearFramebuffer();
            font_renderer.clearFramebuffer();

            player.loadKeyInput(key_input);
            player.loadMouseInput(mouse_input);


            world_0.loadChunks(glm::ivec3(player.getPosition()), 1);

            lights.back() = player.getPosition();

            if(mouse_input.getButtonState(UND_MOUSE_1)) {

                world_0.setCell(TCell<int>(glm::ivec3(player.getPosition() - glm::vec3(10, 10, 10)), glm::ivec3(player.getPosition() + glm::vec3(10, 10, 10)), -1));
                world_0.setCell(TCell<int>(glm::ivec3(player.getPosition() - glm::vec3(11, 10, 11)), glm::ivec3(player.getPosition() + glm::vec3(-9, 10, -9)), holz_bretter));
                world_0.setCell(TCell<int>(glm::ivec3(player.getPosition() + glm::vec3(11, 10, 11)), glm::ivec3(player.getPosition() - glm::vec3(-9, 10, -9)), holz_bretter));

            }


            if(mouse_input.getButtonState(UND_MOUSE_2)) {

                world_0.setCell(TCell<int>(glm::ivec3(player.getPosition() + glm::vec3(0, 0, 0)), glm::ivec3(player.getPosition() + glm::vec3(2, 10, 2)), holz));
                world_0.setCell(TCell<int>(glm::ivec3(player.getPosition() + glm::vec3(2, 0, 0)), glm::ivec3(player.getPosition() + glm::vec3(10, 8, 2)), stone_wall));
                world_0.setCell(TCell<int>(glm::ivec3(player.getPosition() + glm::vec3(2, 8, 0)), glm::ivec3(player.getPosition() + glm::vec3(10, 10, 2)), holz));

                //world_0.setCell(TCell<int>(glm::ivec3(player.getPosition() + glm::vec3(10, 0, 0)), glm::ivec3(player.getPosition() + glm::vec3(12, 10, 2)), holz));
            }

            world_0.optChunks();


            //////////////////////////////////////// drawing ////////////////////////////////////////


            renderer.loadCam(player);
            renderer.loadTextureAtlas(texture_atlas);
            renderer.draw(world_0);

            for (glm::vec3& l : lights) {
                renderer.drawLight(l, 0.5f * glm::vec3(1.0, 0.8, 0.8), 100);
            }

            renderer.drawFinalScene();

            drawCrosshair(font_renderer, arial);

            drawInfo(font_renderer, arial, last_time, player, world_0.getChunk(world_0.getChunkPos(glm::ivec3(player.getPosition()))).getCellCount());


            ///////////////////////////// finishing the frame ////////////////////////////////////////

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

void drawInfo(FontRenderer& fr, Font& f, double last_time, Player& p, int cell_count) {

    fr.setFontColor(glm::vec3(0.8f, 0.8f, 0.8f));

    fr.draw(f, "FPS: " + toStr(1 / (getEngineTime() - last_time)), glm::vec2(-1.0f,0.8f));
    fr.draw(f, "Current Chunk has: " + toStr(cell_count) + " Cells", glm::vec2(-1.0f,0.7f));
    fr.draw(f, "Player Position: " + toStr(p.getPosition().x) + " " + toStr(p.getPosition().y) + " " + toStr(p.getPosition().z), glm::vec2(-1.0f,0.6f));
    fr.draw(f, "Player Direction: " + toStr(p.getViewDirection().x) + " " + toStr(p.getViewDirection().y) + " " + toStr(p.getViewDirection().z), glm::vec2(-1.0f,0.50f));
}

