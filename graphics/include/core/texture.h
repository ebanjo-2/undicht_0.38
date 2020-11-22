#ifndef TEXTURE_H
#define TEXTURE_H

#include <core/shared_lib_class.h>
#include <core/buffer_layout.h>
#include <string>

namespace undicht {

    namespace graphics {

        SHARED_LIB_DECL_BASE_CLASS(Texture, TextureBase, createTexture, copyTexture, deleteTexture);

        SHARED_LIB_CLASS(class Texture: public TextureBase {

        public:

            Texture();
            virtual ~Texture();


        public:
            // managing the textures format

            /** determines the size and layout of a single pixel (i.e. UND_VEC4F for RGBA) */
            virtual void setPixelFormat(const core::BufferLayout& format);

            /** @param depth: a texture object may contain multiple 2D textures, like in an array
            * this can be used to store data for a cubemap (with depth = 6) */
            virtual void setSize(int width, int height, int depth = 1);

            virtual void getSize(int& width, int& height);

            virtual void getSize(int& width, int& height, int& depth);

            /** sets whether there should be a mipmap for this texture
            * may speed up the scaling of the texture, i.e. when applied to a 3d object
            * but also uses more vram, so dont use it if u dont want to scale the texture */
            virtual void setUsesMipMap(bool use_mip_map);

            /// how to retrieve color from the texture if it doesnt align with the pixels on the screen
            virtual void setFilteringMethod(int min_filter, int mag_filter);

            /// what to do if color data is requested outside the range of 0-1 for uv components
            virtual void setWrappingMethod(int method);

        public:
            // managing the textures data

            /** stores pixel data for the texture
            * @param data: should contain pixels in the set Pixel Format
            * @param byte_size: the size of the data to store
            * @param depth: for which depth level the data should be set
            * @param mip_map_level: for which mip map level the data should be (with 0 being the biggest/default, 1 being half the size of 0 and so on)
            * @param offset (in bytes): if you dont want to start at the first pixel */
            virtual void setData(char* data, int byte_size, int depth = 0, int mip_map_level = 0, int offset = 0);

            /** will automatically generate a mipmap for the textures set data */
            virtual void generateMipMaps();

        public:
            // managing the access of the texture data in a shader

            /** binds the texture to the variable in a shader with that name
            * so that the textures data can be accessed through that variable */
            virtual void setName(const std::string& name);

        };)

    } // graphics

} // undicht


#endif // TEXTURE_H
