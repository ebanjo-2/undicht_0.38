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

		loader.loadTextureFromFile(data, "res/sand.png");
		cr.setMaterialTexture(sand, data);

        loader.loadTextureFromFile(data, "res/wood_floor.png");
		cr.setMaterialTexture(wood_floor, data);

        loader.loadTextureFromFile(data, "res/dirt.png");
		cr.setMaterialTexture(dirt, data);

        loader.loadTextureFromFile(data, "res/grass.png");
		cr.setMaterialTexture(grass, data);


        EditChunk big_chunk;
        big_chunk.setCell({ 0,0,0, 255,3,255, dirt.m_id });
        big_chunk.setCell({ 15,4,0, 30,0,240, grass.m_id });
        big_chunk.setCell({ 20,5,20, 9,9,9, stone.m_id });
        big_chunk.setCell({ 19,5,31, 13,1,10, wood_floor.m_id });

        std::cout << big_chunk.getCell(0,0,0) << "\n";

        DrawChunk draw_chunk;
        big_chunk.updateDrawChunk(draw_chunk);

        std::cout << sizeof(big_chunk) << "\n";

		while (!window.shouldClose()) {

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
