#ifndef MATERIAL_REGISTER_H
#define MATERIAL_REGISTER_H

#include <material.h>
#include <core/texture.h>
#include <files/model_loading/image_loader.h>


namespace cell {

	class MaterialRegister {

		protected:

			// storing the data of registered materials

			struct RegisteredMaterial {

				std::string name;
				int prefix;

			};

			std::vector<RegisteredMaterial> m_materials;
			std::vector<std::string> m_prefixes;
			undicht::graphics::Texture m_texture_atlas; // contains the texture data of all materials

		public:
			// managing materials

			Material addMaterial(const std::string& prefix, const std::string& name);

			void setMaterialTexture(const Material& mat, undicht::tools::TextureData& data);

		public:

			const undicht::graphics::Texture& getTextureAtlas() const;

		public:

			MaterialRegister();
			virtual ~MaterialRegister();
	};


} // cell

#endif // MATERIAL_REGISTER_H