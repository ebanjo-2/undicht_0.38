#include <iostream>
#include <window/window.h>
#include <core/graphics_core.h>
#include <core/renderer.h>

#include <files/model_loading/collada/collada_file.h>

using namespace undicht;
using namespace graphics;
using namespace tools;


int main() {
	

	Window window(800, 600, "HELLO WORLD!");

	{

		MeshData mesh_data;
		TextureData texture_data;

		/*ColladaFile model_loader("res/SupertuxTex.dae");
		model_loader.getMesh(mesh_data);*/

		VertexBuffer vbo;
		vbo.setLayout(mesh_data.vertex_layout);
		vbo.setData(mesh_data.vertices);

		File shader_file("res/shader.glsl");
		std::string shader_src;
		shader_file.getAll(shader_src);

		Shader shader;
		shader.loadSource(shader_src);

		Renderer renderer;

		while (!window.shouldClose()) {

			renderer.clearFramebuffer(0.1, 0.6, 0.1, 1.0F);

			window.update();
		}


		window.close();

	}

	return 0;
}
