#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include <core/texture.h>


namespace cell {

    class TextureAtlas {
            /** stores a lot of textures ( 16*16 )
            * in an array of 256 * 256 textures */

        public:

            // name in shader: "texture_atlas"
            undicht::graphics::Texture m_tex;

            unsigned int m_tex_count = 0; // id of the next texture that gets added (= total texture count)

        public:

            int addTexture(const undicht::graphics::Texture& tex);

            int addTexture(const std::string& file_name);

            TextureAtlas();
            virtual ~TextureAtlas();

    };

} // cell

#endif // TEXTURE_ATLAS_H
