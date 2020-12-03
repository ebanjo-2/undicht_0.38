#ifdef TEXTURE_H // this file should only be included via core/texture.h

#ifndef GL33_TEXTURE_H
#define GL33_TEXTURE_H

#include <string>
#include "shared_id.h"

namespace undicht {

	namespace graphics {

		namespace gl33 {

			class Texture : public interf::Texture {

			public:
				// members

				tools::SharedID m_id = 0;
				unsigned int m_width = 0;
				unsigned int m_height = 0;
				unsigned int m_depth = 0;
				int m_type = 0;

				int m_mag_filter = -1;
				int m_min_filter = -1;
				int m_wrapping_method = -1;

				int m_pixel_layout = 0; // like rgb, rgba
				int m_memory_format = 0; // stores how big each component of the pixel is

				std::string m_name; // uniform name


			public:
				// managing the textures format

				/** determines the size and layout of a single pixel (i.e. UND_VEC4F for RGBA) */
				virtual void setPixelFormat(const tools::BufferLayout& format);

				/** @param depth: a texture object may contain multiple 2D textures, like in an array
				* this can be used to store data for a cubemap (with depth = 6) */
				virtual void setSize(int width, int height, int depth);

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

			public:
				// opengl only functions / members

				virtual void bind(unsigned int target = 0) const;

				bool m_size_set = false;
				bool m_layout_set = false;


				/** updates the texture to the set layout and size */
				void updateFormat();

				/** @return an array containing the type for each component of the color */
				std::array<int, 4> getComponentTypes(const tools::BufferLayout& format);

				void setOpenglFormat(const std::array<int, 4>& component_types);
				void setOpenglFormat(int layout, int memory_format); // directly setting the format

				/// translates an undicht filtering type to an opengl one
				int getGLFilteringMethod(int und_filtering_method);


			public:

				Texture();
				virtual ~Texture();

			};

		} // gl33

	} // graphics

} // undicht


#endif // GL33_TEXTURE_H

#endif // TEXTURE_H
