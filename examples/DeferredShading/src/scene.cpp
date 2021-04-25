#include <scene.h>
#include <files/model_loading/collada/collada_file.h>
#include <buffer_layout.h>

std::vector<VertexBuffer*> vbos;
std::vector<Texture*> texs;
std::vector<int> tex_ids;

void initScene() {



	std::vector<MeshData> mesh_data;
	std::vector<TextureData> texture_data;

	ColladaFile model_loader;
	model_loader.open("res/sponza_collada/sponza.dae");
	//model_loader.open("res/SupertuxTex.dae");

	model_loader.loadAllMeshes(mesh_data);
	model_loader.loadAllTextures(texture_data);

	for (MeshData& md : mesh_data) {

		vbos.emplace_back(new VertexBuffer);
		vbos.back()->setLayout(md.vertex_layout);
		vbos.back()->setData(md.vertices);

		tex_ids.push_back(md.color_texture);
	}

	for (TextureData& td : texture_data) {

		texs.emplace_back(new Texture);
		texs.back()->setName("color"); // var name in shader

		if (td.pixel_layout.getTotalSize()) {
			// setting texture data

			texs.back()->setPixelFormat(td.pixel_layout);
			texs.back()->setSize(td.width, td.height, 1);
			texs.back()->setData(td.pixels.data(), td.pixels.size());

			texs.back()->generateMipMaps();

		}
		else {
			// setting blank texture data ("texture missing")

			unsigned char blank[3] = { 255, 50, 100 }; // pink

			texs.back()->setPixelFormat(BufferLayout({ UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR }));
			texs.back()->setSize(1, 1, 1);
			texs.back()->setData((char*)blank, 3 * sizeof(unsigned char));
		}

	}

}

void termScene() {

	for (VertexBuffer* vbo : vbos) {
		delete vbo;
	}

	for (Texture* tex : texs) {
		delete tex;
	}
}
