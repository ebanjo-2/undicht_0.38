#include <rendering/texture_atlas.h>

#include <files/model_loading/image_loader.h>

#include <types.h>
#include <iostream>

namespace cell {

    using namespace undicht;
    using namespace graphics;
    using namespace tools;

    const int MIN_ARRAY_DEPTH = 2; // initial depth of the array (256 textures per layer)

    TextureAtlas::TextureAtlas() {

        m_tex.setName("texture_atlas");
        m_tex.setPixelFormat(BufferLayout({UND_UINT8, UND_UINT8, UND_UINT8}));
        m_tex.setSize(256, 256, MIN_ARRAY_DEPTH);
        m_tex.setFilteringMethod(UND_NEAREST, UND_NEAREST);


    }


    TextureAtlas::~TextureAtlas() {


    }

    int TextureAtlas::addTexture(const undicht::graphics::Texture& tex) {


    }

    int TextureAtlas::addTexture(const std::string& file_name) {

        TextureData data;
        ImageLoader image;
        image.loadTextureFromFile(data, file_name);

        if(data.height != 16) return -1;
        if(data.width != 16) return -2;
        if(data.pixel_layout.getTotalSize() != 3) return -3;

        unsigned int layer = (m_tex_count + 1 ) / 256;
        unsigned int tex_in_layer = m_tex_count % 256;

        unsigned int offsetx = (tex_in_layer % 16) * 16;
        unsigned int offsety = (tex_in_layer / 16) * 16; // not redundant, integers !

        m_tex.setData(data.pixels.data(), 16 * 16 * 3, layer, 0, offsetx, offsety, 16, 16);

        m_tex_count += 1;

        return m_tex_count - 1;
    }

} // cell
