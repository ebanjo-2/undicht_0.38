#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
// using stb for implementation


#include "ttfont_file.h"
#include <stdio.h>


#include "event_logger.h"


namespace undicht {

    namespace tools {

        TTFontFile::TTFontFile() {

        }


        TTFontFile::TTFontFile(const std::string& file_name) {

            open(file_name);
        }


        TTFontFile::~TTFontFile() {

            close();
        }

        bool TTFontFile::open(const std::string& file_name) {

            if(!File::open(file_name)) return false;

            unsigned int file_size = getAll(m_ttf_buffer);

            // load the font data to the stb fontinfo class
            //stbtt_InitFont(&m_font_info, (const unsigned char*)m_ttf_buffer, stbtt_GetFontOffsetForIndex((const unsigned char*)m_ttf_buffer,0));

            return true;
        }

        void TTFontFile::close() {

            delete[] m_ttf_buffer;
            File::close();
        }

        void TTFontFile::genFontMap(unsigned char*& loadTo_FontMap, int& w, int& h, // the generated fontmap ( the loadTo_FontMap has to be deleted manually!!!!)
                              std::vector<FontCharData>& loadTo_CharData,
                              unsigned int first_char, unsigned int last_char,        // the characters to generate the fontmap for
                              unsigned int font_pixel_height,                         // height of font in pixels
                              int max_bitmap_width                                    // max width for the fontmap
                             ) {

            // assuming most characters width is smaller/equal to their height
            unsigned int font_pixel_width = font_pixel_height;
            unsigned int num_chars = last_char - first_char + 1;

            unsigned int num_rows = ((font_pixel_width * num_chars) / max_bitmap_width) + 1;

            // calculating width and height of the fontmap
            w = max_bitmap_width; // inefficient in most cases
            h = num_rows * (font_pixel_height + 1); // required number of lines * height per line

            unsigned char* revers_fm = new unsigned char[w * h];
            loadTo_FontMap = new unsigned char[w * h];

            // getting stb to generate a fontmap
            stbtt_bakedchar* char_data = new stbtt_bakedchar[num_chars]; // contains info about the chars such as size and position
            int result = stbtt_BakeFontBitmap((const unsigned char*)m_ttf_buffer, 0, font_pixel_height, revers_fm, w, h, first_char, num_chars, char_data);

            // checking if the fontmap was successfully generated
            if(result > 0) h = result; // the fontmap was to big. to bad!
            if(result > (-1 * num_chars)) EventLogger::storeNote(Note(UND_ERROR, "failed to place all chars on fontmap", UND_CODE_ORIGIN));
            if(!result) EventLogger::storeNote(Note(UND_ERROR, "failed to place any chars on fontmap", UND_CODE_ORIGIN));

            // stb has the y coordinate of the fontmap increase in the downward direction (why would u do that tho?)
            // so the fontmap needs to be reversed vertically

            for(int i = 0; i < h; i++) {

                std::copy(revers_fm + i * w, revers_fm + (i + 1) * w - 1, loadTo_FontMap + w * h - (i + 1) * w);
            }

            // storing the char data in FontCharData
            for(int i = first_char; i <= last_char; i++) {

                stbtt_bakedchar* stbchar = char_data + i;
                FontCharData mychar;

                mychar.m_value = i;
                mychar.m_fm_pos0 = glm::vec2(stbchar->x0, stbchar->y0) / glm::vec2(w, h); // relative coordinates ranging from 0 to 1
                mychar.m_fm_pos1 = glm::vec2(stbchar->x1, stbchar->y1) / glm::vec2(w, h);
                mychar.m_advance = stbchar->xadvance / w;
                mychar.m_offset = glm::vec2(stbchar->xoff, stbchar->yoff) / glm::vec2(w, h);

                // reversing uv coords vertically
                float temp = 1 - mychar.m_fm_pos1.y;
                mychar.m_fm_pos1.y = 1 - mychar.m_fm_pos0.y;
                mychar.m_fm_pos0.y = temp;

                loadTo_CharData.push_back(mychar);
            }

            delete[] char_data;
        }

        float TTFontFile::getFontScale(int font_pixel_height) {
            /** used for scaling offsets and advances */

            stbtt_fontinfo font; // stb class for holding the font data
            stbtt_InitFont(&font, (const unsigned char*)m_ttf_buffer, stbtt_GetFontOffsetForIndex((const unsigned char*)m_ttf_buffer,0));

            return stbtt_ScaleForPixelHeight(&font, font_pixel_height);
        }



    } // tools

} // undicht
