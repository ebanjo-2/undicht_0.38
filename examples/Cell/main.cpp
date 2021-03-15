#include <iostream>
#include <vector>
#include <array>

#include <window/window.h>

#include <material.h>

#include <rendering/cell_renderer.h>

#include <camera/camera_control.h>
#include <files/model_loading/image_loader.h>

#include <chunks/edit_chunk.h>

using namespace undicht;
using namespace graphics;

using namespace std;
using namespace cell;

int main() {

    Window window(1600, 900, "Cell");

    {

        initCam();
        key_input->setInputWindow(&window);
        mouse_input->setInputWindow(&window);

        window.setCursorVisible(false);

        CellRenderer cr;
        PerspectiveCamera3D cam;
        cam.setAspectRatio(1600.0f / 900.0f);
        cam.setPosition(glm::vec3(1, 1, 1));

        // materials
        Material stone = cr.registerMaterial("default", "stone");
        Material sand = cr.registerMaterial("default", "sand");
        Material wood_floor = cr.registerMaterial("default", "wood_floor");
        Material dirt = cr.registerMaterial("default", "dirt");
        Material grass = cr.registerMaterial("default", "grass");



        ImageLoader loader;
        TextureData data;

        loader.loadTextureFromFile(data, "res/stone.png");
        cr.setMaterialTexture(stone, data);

        std::cout << stone.getID() << "\n";

        loader.loadTextureFromFile(data, "res/sand.png");
        cr.setMaterialTexture(sand, data);

        loader.loadTextureFromFile(data, "res/wood_floor.png");
        cr.setMaterialTexture(wood_floor, data);

        loader.loadTextureFromFile(data, "res/dirt.png");
        cr.setMaterialTexture(dirt, data);

        loader.loadTextureFromFile(data, "res/grass.png");
        cr.setMaterialTexture(grass, data);


        DrawChunk draw_chunk;

        EditChunk big_chunk;
        /*Cell c(stone.m_id);
        c.setPosition({0,0,0});
        c.setSize({255,255,255});

        big_chunk.setCell(c, &draw_chunk);*/

        big_chunk.setCell(Cell({ 0,0,0}, { 255,3,255}, dirt.m_id), &draw_chunk);
        big_chunk.setCell(Cell({ 15,4,1}, {30,2,240}, grass.m_id), &draw_chunk);
        /*big_chunk.setCell(Cell({ 20,5,20}, {13,1,10}, stone.m_id), &draw_chunk);
        //big_chunk.setCell({ 20,15,20, 9,9,9, stone.m_id }, &draw_chunk);
        big_chunk.setCell(Cell({ 19,5,31}, {13,1,10}, wood_floor.m_id), &draw_chunk);*/

        draw_chunk.updateCellBuffer();


        while (!window.shouldClose()) {

            /*unsigned int x = cam.getPosition().x;
            unsigned int y = cam.getPosition().y;
            unsigned int z = cam.getPosition().z;

            x = std::min(x, (uint)242);
            y = std::min(y, (uint)242);
            z = std::min(z, (uint)242);

            x = std::max(x, (uint)13);
            y = std::max(y, (uint)13);
            z = std::max(z, (uint)13);

            c.setPosition({x - 13,y - 13,z - 13 });
            c.setSize({26,26,26});
            c.mat = 0;
            big_chunk.setCell(c, &draw_chunk);
            draw_chunk.updateCellBuffer();*/

            cr.clearFramebuffer();

            cr.loadCam(cam);
            cr.drawChunk(draw_chunk, 0,0,0);

            moveCam(cam);

            mouse_input->updateCursorOffset();
            window.update();
        }

        termCam();
    }

    window.close();

    return 0;
}
