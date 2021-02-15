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

	Window window(800, 600, "Cell");

	{

		initCam();
		key_input->setInputWindow(&window);
		mouse_input->setInputWindow(&window);

		CellRenderer cr;
		PerspectiveCamera3D cam;
		cam.setPosition(glm::vec3(1, 1, 1));

		// materials
		Material stone = cr.registerMaterial("default", "stone");
		Material sand = cr.registerMaterial("default", "sand");
		
		ImageLoader loader;
		TextureData data;

		loader.loadTextureFromFile(data, "res/stone.png");
		cr.setMaterialTexture(stone, data);

		loader.loadTextureFromFile(data, "res/iron_ore.png");
		cr.setMaterialTexture(sand, data);


		// cells
		CellChunk<4> cells[5];
		cells[0] = std::array<unsigned int, 7>({ 0,0,0, 0,0,0, 0 });
		cells[1] = std::array<unsigned int, 7>({ 0,1,1, 1,1,1, 0 });
		cells[2] = std::array<unsigned int, 7>({ 0,3,3, 1,1,1, 1 });
		cells[3] = std::array<unsigned int, 7>({ 0,5,5, 1,0,1, 1 });
		cells[4] = std::array<unsigned int, 7>({ 0,6,0, 1,1,1, 1 });

		CompactChunk<4> chunk;
		
		for (const CellChunk<4>& cc : cells) {

			chunk.m_cells.push_back(cc);

		}


		while (!window.shouldClose()) {

			cr.clearFramebuffer();

			cr.loadCam(cam);
			cr.drawChunk(chunk);

			moveCam(cam);

			mouse_input->updateCursorOffset();
			window.update();
		}

		termCam();
	}

	window.close();

    return 0;
}
