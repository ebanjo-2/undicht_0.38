#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <vector>
#include <string>
#include <buffer_layout.h>

namespace undicht {

	namespace tools {

		struct TextureData {

			std::vector<char> pixels;

			int width;
			int height;
			int depth;

			BufferLayout pixel_layout;

		};

		class ImageLoader {
			/** a class to load image files */

		public:

			ImageLoader() = default;
			virtual ~ImageLoader() = default;

		public:
			// loading textures from files

			void loadTextureFromFile(TextureData& loadTo_texture, const std::string& file_name);

		};

	} // tools

} // undicht


#endif // IMAGE_LOADER_H
