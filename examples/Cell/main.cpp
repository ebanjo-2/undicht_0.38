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

using namespace undicht;
using namespace graphics;
using namespace tools;
using namespace user_input;

using namespace cell;

void drawCrosshair(FontRenderer& fr, Font& f);



int main(int argc, char **argv) {

    const int WINDOW_WIDTH = 1920;
    const int WINDOW_HEIGHT = 1080;

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "HELLO WORLD");
    window.setWindowMode(true,true);
    window.setCursorVisible(false);

    initEngineTime();

    {

        Font arial("res/arial.ttf", 25);

        KeyInput key_input;
        key_input.setInputWindow(&window);

        MouseInput mouse_input;
        mouse_input.setInputWindow(&window);

        Player player(glm::vec3(0,25,0));

        CellRenderer renderer;
        FontRenderer font_renderer;

        renderer.setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);
        font_renderer.setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);

        TextureAtlas texture_atlas;
        //int dirt = texture_atlas.addTexture("res/dirt.png");
        //int grass = texture_atlas.addTexture("res/grass.png");
        //int iron_ore = texture_atlas.addTexture("res/iron_ore.png");
       // int wood_floor = texture_atlas.addTexture("res/wood_floor.png");

        int dirt = texture_atlas.addTexture("res/dirt32.png");
        int grass = texture_atlas.addTexture("res/grass32.png");
        int stone = texture_atlas.addTexture("res/stone32.png");

        texture_atlas.m_tex.generateMipMaps();

        u8vec3 v1(10, 0, 20);

        /*Chunk chunk_0;

        chunk_0.setCells({
            Cell(u8vec3(0,0,0), u8vec3(255,250,255), 0),
            Cell(u8vec3(3,5,3), u8vec3(12,12,12), 0),
            Cell(u8vec3(9,5,5), u8vec3(15,10,10), 1),
            Cell(u8vec3(6,2,6), u8vec3(7,5,7), 1),
        });*/

        World world_0;


        double last_time = 0;
        double edit_time = 0;
        double draw_time = 0;


        //chunk_0.initDrawBuffer();

        Thread* opt_thread = 0;
        double thread_start_time = 0;
        Chunk* old_chunk = 0;
        Chunk* opt_chunk = 0;

        while(!(window.shouldClose() || key_input.getKeyState(UND_KEY_ESC))) {

            renderer.clearFramebuffer();
            //font_renderer.clearFramebuffer();

            player.loadKeyInput(key_input);
            player.loadMouseInput(mouse_input);

            world_0.loadChunks(glm::ivec3(player.getPosition()), 1);

            if(mouse_input.getButtonState(UND_MOUSE_1)) {
                edit_time = getEngineTime();
                world_0.setCell(TCell<int>(glm::ivec3(player.getPosition() - glm::vec3(10, 10, 10)), glm::ivec3(player.getPosition() + glm::vec3(10, 10, 10)), -1));
                edit_time = getEngineTime() - edit_time;
            }

            if(mouse_input.getButtonState(UND_MOUSE_2)) {
                edit_time = getEngineTime();
                world_0.setCell(TCell<int>(glm::ivec3(player.getPosition() - glm::vec3(10, 10, 10)), glm::ivec3(player.getPosition() + glm::vec3(10, 10, 10)), 0));
                edit_time = getEngineTime() - edit_time;
            }

            if((!opt_chunk) && key_input.getKeyState(UND_KEY_O)) {

                old_chunk = &world_0.getChunk(world_0.getChunkPos(glm::ivec3(player.getPosition())));
                opt_chunk = new Chunk();

                opt_thread = new Thread(optimizeChunk, old_chunk, opt_chunk);
                thread_start_time = getEngineTime();

            }

            if(opt_thread && opt_thread->hasFinished()) {

                delete opt_thread;
                opt_thread = 0;

                *old_chunk = *opt_chunk;

                for(int i = 0; i < opt_chunk->m_cells.size(); i++) {

                    old_chunk->updateDrawBuffer(i);
                }

                old_chunk = 0;

                delete opt_chunk;
                opt_chunk = 0;
            }


            renderer.loadCam(player);
            renderer.loadTextureAtlas(texture_atlas);


            draw_time = getEngineTime();

            renderer.draw(world_0);
            renderer.drawLight(glm::vec3(0,22,0), glm::vec3(1,0.8,0), 22);
            renderer.drawLight(glm::vec3(50,22,0), glm::vec3(1,0.8,0), 22);
            renderer.drawLight(glm::vec3(20,22,40), glm::vec3(1,0.8,0), 22);
            renderer.drawLight(player.getPosition(), glm::vec3(3,3.0,3.0), 40);

            renderer.drawFinalScene();

            draw_time = getEngineTime() - draw_time;

            font_renderer.setFontColor(glm::vec3(0.8f, 0.8f, 0.8f));
            drawCrosshair(font_renderer, arial);

            font_renderer.draw(arial, "FPS: " + toStr(1 / (getEngineTime() - last_time)), glm::vec2(-1.0f,0.8f));
            font_renderer.draw(arial, "Draw Time: " + toStr(draw_time), glm::vec2(-1.0f,0.75f));
            font_renderer.draw(arial, "Loaded Chunks: " + toStr(world_0.m_loaded_chunks.size()), glm::vec2(-1.0f,0.7f));

            font_renderer.draw(arial, "Player Position: " + toStr(player.getPosition().x) + " " + toStr(player.getPosition().y) + " " + toStr(player.getPosition().z), glm::vec2(-1.0f,0.6f));
            font_renderer.draw(arial, "Current Chunk: " + toStr(world_0.getChunkPos((glm::ivec3)player.getPosition()).x) + " " + toStr(world_0.getChunkPos((glm::ivec3)player.getPosition()).y) + " " + toStr(world_0.getChunkPos((glm::ivec3)player.getPosition()).z), glm::vec2(-1.0f,0.55f));
            font_renderer.draw(arial, "Player Direction: " + toStr(player.getViewDirection().x) + " " + toStr(player.getViewDirection().y) + " " + toStr(player.getViewDirection().z), glm::vec2(-1.0f,0.50f));

            last_time = getEngineTime();


            key_input.clearKeyLists();
            mouse_input.updateCursorOffset();
            mouse_input.clearButtonLists();
            window.update();


        }

        if(opt_thread)
            delete opt_thread;

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
