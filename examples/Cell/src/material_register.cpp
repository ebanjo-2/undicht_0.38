
#include <material_register.h>
#include <iostream>

#include <buffer_layout.h>


using namespace undicht;
using namespace graphics;
using namespace tools;


namespace cell {

	MaterialRegister::MaterialRegister() {

		m_texture_atlas.setName("texture_atlas");
		m_texture_atlas.setSize(16, 16, 16); // 16 materials is the limit for now

		BufferLayout layout({ UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR });
		m_texture_atlas.setPixelFormat(layout);

		m_texture_atlas.setFilteringMethod(UND_NEAREST, UND_NEAREST);

		// making sure the material with id 0 is void
		addMaterial("default", "void");
	}

	MaterialRegister::~MaterialRegister() {

	}

	///////////////////////////////////////// managing materials /////////////////////////////////////////

	Material MaterialRegister::addMaterial(const std::string& prefix, const std::string& name) {

		RegisteredMaterial reg_mat;
		reg_mat.name = name;

		// searching for the prefix
		for (int i = 0; i < m_prefixes.size(); i++) {

			if (!prefix.compare(m_prefixes.at(i))) {

				reg_mat.prefix = i;
				break;
			}

		}

		Material new_mat;
		new_mat.setName(prefix, name);
		new_mat.setID(m_materials.size());

		m_materials.push_back(reg_mat);

		return new_mat;
	}

	void MaterialRegister::setMaterialTexture(const Material& mat, undicht::tools::TextureData& data) {

		if (mat.m_id > 15) {

			std::cout << "ERROR: maximum material count is 16" << "\n";
			return;
		}

		m_texture_atlas.setData(data.pixels.data(), sizeof(data.pixels), mat.m_id);
		m_texture_atlas.generateMipMaps();
	}

	const Texture& MaterialRegister::getTextureAtlas() const{

		return m_texture_atlas;
	}

} // cell