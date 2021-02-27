#include "image_loader.h"
#include "event_logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


namespace undicht {

	namespace tools {


		//////////////////////////////////////////////////// loading textures from files ////////////////////////////////////////////////////

		void ImageLoader::loadTextureFromFile(TextureData& loadTo_texture, const std::string& file_name) {

			stbi_set_flip_vertically_on_load(true);
			
			int nr_channels;

			//loading the image data
			unsigned char* data = stbi_load(file_name.data(), &loadTo_texture.width, &loadTo_texture.height, &nr_channels, 0);

			if (data) { //testing whether the image could be loaded or not

				// storing the data in the TextureData object
				loadTo_texture.pixels.insert(loadTo_texture.pixels.begin(), data, data + loadTo_texture.width * loadTo_texture.height * nr_channels * sizeof(char));

				// storing the pixel layout
				for (int i = 0; i < nr_channels; i++) {

					loadTo_texture.pixel_layout.m_types.push_back(UND_UNSIGNED_CHAR); // assuming that every color channel is 1 byte
				}

			} else {

				EventLogger::storeNote(Note(UND_ERROR, "CORE:MODEL_LOADER:ERROR: failed to read image: " + file_name, UND_CODE_ORIGIN));

			}

			stbi_image_free(data);

		}


	} // tools

} // tools