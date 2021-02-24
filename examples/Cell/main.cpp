#include <iostream>
#include <vector>
#include <array>

#include <window/window.h>

#include <material.h>
#include <cell.h>
#include <chunk.h>

#include <rendering/cell_renderer.h>

#include <camera/camera_control.h>
#include <files/model_loading/image_loader.h>

using namespace undicht;
using namespace graphics;

using namespace std;

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


		// cells
		CellChunk<8> cells[5];
		cells[0] = std::array<unsigned int, 7>({ 0,0,0, 255,3,255, 3 });
		cells[1] = std::array<unsigned int, 7>({ 15,4,0, 30,0,240, 4 });
		cells[2] = std::array<unsigned int, 7>({ 20,5,20, 9,9,9, 0 });
		cells[3] = std::array<unsigned int, 7>({ 19,5,31, 13,1,10, 2 });
		cells[4] = std::array<unsigned int, 7>({ 13,4,5, 0,0,0, 1 });

		CompactChunk<8> chunk;

		for (const CellChunk<8>& cc : cells) {

			chunk.m_cells.push_back(cc);

		}


		cr.updateChunk(chunk, 0,0,0);
        cr.updateChunk(chunk, 0,0,1);

		while (!window.shouldClose()) {

			cr.clearFramebuffer();

			cr.loadCam(cam);
			cr.drawChunks();

			moveCam(cam);

			mouse_input->updateCursorOffset();
			window.update();
		}

		termCam();
	}

	window.close();

    return 0;
}
