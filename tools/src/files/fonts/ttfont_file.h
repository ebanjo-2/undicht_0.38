#ifndef TTFONT_FILE_H
#define TTFONT_FILE_H

#include <glm/glm.hpp>

#include <vector>
#include <string>

#include <files/file.h>




namespace undicht {

    namespace tools {

        class FontCharData {
            // all coordinates are in the range 0 to 1
            // so that they can easily be used as uvs

            public:

            char m_value = 0;

            glm::vec2 m_fm_pos0; // lower left on fontmap
            glm::vec2 m_fm_pos1; // upper right on fontmap

            float m_advance = 0;
            glm::vec2 m_offset; // offset to the char position in a line of text

        };

        class TTFontFile : public File {
                /** a class for reading TrueType Fonts */

            public:

                char* m_ttf_buffer = 0; // the file read into memory

            public:

                virtual bool open(const std::string& file_name);
                virtual void close();

                void genFontMap(unsigned char*& loadTo_FontMap, int& w, int& h, // the generated fontmap ( the loadTo_FontMap has to be deleted manually!!!!)
                    std::vector<FontCharData>& loadTo_CharData,
                    unsigned int first_char, unsigned int last_char,        // the characters to generate the fontmap for
                    unsigned int font_pixel_height,                         // height of font in pixels
                    int max_bitmap_width = 1024                             // max width for the fontmap
                );

                /** used for scaling offsets and advances */
                float getFontScale(int font_pixel_height);


            public:

                TTFontFile();
                TTFontFile(const std::string& file_name);
                virtual ~TTFontFile();

        };


    }

} // undicht

#endif // TTFONT_FILE_H
