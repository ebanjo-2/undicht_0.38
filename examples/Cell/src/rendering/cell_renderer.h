#include <core/renderer.h>

#include <core/shader.h>

#include <camera/perspective_camera_3d.h>

#include <cell.h>
#include <chunk.h>
#include <material.h>

#include <files/model_loading/image_loader.h>
#include <core/texture.h>




class CellRenderer : public undicht::graphics::Renderer {

public:
	// storing the data of registered materials

	struct RegisteredMaterial {

		std::string name;
		int prefix;

	};

	static std::vector<RegisteredMaterial> s_materials;

	static std::vector<std::string> s_prefixes;

	static undicht::graphics::Texture* s_texture_atlas; // contains the texture data of all materials


public:

	// the buffer that stores the cells of a chunk
	static undicht::graphics::VertexBuffer* s_buffer;

	// camera uniforms
	static undicht::graphics::Uniform* s_proj;
	static undicht::graphics::Uniform* s_view;

	static undicht::graphics::Shader* s_shader;

	static int s_renderer_count;

	static void init();
	static void term();

public:
	// managing materials

	static Material registerMaterial(const std::string& prefix, const std::string& name);

	static void setMaterialTexture(const Material& mat, undicht::tools::TextureData& data);

public:
	// drawing

	void loadCam(undicht::graphics::PerspectiveCamera3D& cam);

	void drawChunk(const CompactChunk<4>& c);

	CellRenderer();
	virtual ~CellRenderer();

};

