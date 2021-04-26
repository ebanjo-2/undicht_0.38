#include <iostream>
#include <vector>
#include <array>

#include <window/window.h>

#include <material.h>
#include <material_register.h>

#include <rendering/cell_renderer.h>
#include <rendering/cell_gbuffer.h>

#include <rendering/final/final_renderer.h>

#include <camera/camera_control.h>
#include <files/model_loading/image_loader.h>

#include <chunks/edit_chunk.h>

#include <undicht_time.h>

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
		window.setWindowMode(true, false);

		CellRenderer cr;
		cr.setViewport(1680, 1050);

		PerspectiveCamera3D cam;
		cam.setAspectRatio(1600.0f / 900.0f);
		cam.setPosition(glm::vec3(1, 1, 1));

		CellGBuffer gbuffer(1680, 1050);
		cr.submit(&gbuffer);

		FinalRenderer fr;
		fr.setViewport(1680, 1050);

		// materials

		MaterialRegister material_register;

		Material stone = material_register.addMaterial("default", "stone");
		Material sand = material_register.addMaterial("default", "sand");
		Material wood_floor = material_register.addMaterial("default", "wood_floor");
		Material dirt = material_register.addMaterial("default", "dirt");
		Material grass = material_register.addMaterial("default", "grass");


		ImageLoader loader;
		TextureData data;

		loader.loadTextureFromFile(data, "res/stone.png");
		material_register.setMaterialTexture(stone, data);

		std::cout << stone.getID() << "\n";

		loader.loadTextureFromFile(data, "res/sand.png");
		material_register.setMaterialTexture(sand, data);

		loader.loadTextureFromFile(data, "res/wood_floor.png");
		material_register.setMaterialTexture(wood_floor, data);

		loader.loadTextureFromFile(data, "res/dirt.png");
		material_register.setMaterialTexture(dirt, data);

		loader.loadTextureFromFile(data, "res/grass.png");
		material_register.setMaterialTexture(grass, data);


		DrawChunk draw_chunks[2];
		draw_chunks[0].m_position = glm::ivec3(0, 0, 0);
		draw_chunks[1].m_position = glm::ivec3(255, 0, 0);


		EditChunk edit_chunks[2];

		//Cell c(stone.m_id);
		Cell c(0);

		//big_chunk.setCell(c, &draw_chunk);
		edit_chunks[0].setCell(Cell(glm::uvec3(0, 0, 0), glm::uvec3(255, 60, 255), stone.m_id),	 &draw_chunks[0]);
		edit_chunks[0].setCell(Cell(glm::uvec3(0, 60,0), glm::uvec3(255, 70, 255), dirt.m_id),	 &draw_chunks[0]);
		edit_chunks[0].setCell(Cell(glm::uvec3(0, 70, 0), glm::uvec3(255, 71, 255), grass.m_id), &draw_chunks[0]);
		edit_chunks[0].setCell(Cell(glm::uvec3(4, 71, 4), glm::uvec3(15, 80, 15), stone.m_id),	 &draw_chunks[0]);

		edit_chunks[1].setCell(Cell(glm::uvec3(0, 0, 0), glm::uvec3(255, 60, 255), stone.m_id),	 &draw_chunks[1]);
		edit_chunks[1].setCell(Cell(glm::uvec3(0, 60, 0), glm::uvec3(255, 70, 255), dirt.m_id),	 &draw_chunks[1]);
		edit_chunks[1].setCell(Cell(glm::uvec3(0, 70, 0), glm::uvec3(255, 71, 255), grass.m_id), &draw_chunks[1]);
		edit_chunks[1].setCell(Cell(glm::uvec3(4, 71, 4), glm::uvec3(15, 80, 15), stone.m_id),	 &draw_chunks[1]);

        /*big_chunk.setCell(Cell({ 20,5,20}, {13,1,10}, stone.m_id), &draw_chunk);
        //big_chunk.setCell({ 20,15,20, 9,9,9, stone.m_id }, &draw_chunk);
        big_chunk.setCell(Cell({ 19,5,31}, {13,1,10}, wood_floor.m_id), &draw_chunk);*/

        draw_chunks[0].updateCellBuffer();
		draw_chunks[1].updateCellBuffer();


        while (!window.shouldClose()) {

            unsigned int x = cam.getPosition().x;
            unsigned int y = cam.getPosition().y;
            unsigned int z = cam.getPosition().z;

            x = std::min(x % 255, unsigned int(245));
            y = std::min(y % 255, unsigned int(245));
            z = std::min(z % 255, unsigned int(245));

            x = std::max(x, unsigned int(10));
            y = std::max(y, unsigned int(10));
            z = std::max(z, unsigned int(10));

            c.setPosition(glm::uvec3(x - 10,y - 10,z - 10));
            c.setSize(glm::uvec3(20, 20, 20));


			if (mouse_input->getButtonState(UND_MOUSE_1)) {
				// adding a 20*20*20 cube of stone around the camera
				c.setMaterial(wood_floor.m_id);

				if (cam.getPosition().x <= 255) {

					edit_chunks[0].setCell(c, &draw_chunks[0]);

				} else {

					edit_chunks[1].setCell(c, &draw_chunks[1]);
				}

			}
			
			if (mouse_input->getButtonState(UND_MOUSE_2)) {
				// adding a 20*20*20 cube of nothing around the camera
				c.setMaterial(0);

				if (cam.getPosition().x <= 255) {

					edit_chunks[0].setCell(c, &draw_chunks[0]);

				} else {

					edit_chunks[1].setCell(c, &draw_chunks[1]);
				}
			}

			draw_chunks[0].updateCellBuffer();
			draw_chunks[1].updateCellBuffer();


            cr.clearFramebuffer(0,0,0,0); // all zeros, no false info for deferred rendering

            cr.loadCam(cam);

			for (DrawChunk& c : draw_chunks) {

				cr.drawChunk(c);
			}

			fr.clearFramebuffer();
			fr.drawFinal(gbuffer, material_register);

            moveCam(cam);

            mouse_input->updateCursorOffset();
            window.update();

			std::cout << getFPS() << "\n";
        }

        termCam();
    }

    window.close();

    return 0;
}
