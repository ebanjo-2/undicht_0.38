#include <stdio.h>
#include <iostream>

#include "font.h"

#include "string_tools.h"
#include "types.h"
#include "files/fonts/ttfont_file.h"

namespace undicht {

    namespace graphics {

        using namespace tools;

        Font::Font() {

        }

        Font::Font(const std::string& file_name, unsigned int font_height) {

            loadFromFile(file_name, font_height);
        }

        Font::~Font() {

        }


        ////////////////////////////////////// setting data //////////////////////////////////////

        void Font::loadFromFile(const std::string& file_name, unsigned int font_height) {
            /** @param font_height: the height of the font in pixels */

            unsigned char* fontmap_data = 0;
            int w = 0, h = 0;

            if(hasFileType(file_name, ".ttf")) {
                TTFontFile file(file_name);
                file.genFontMap(fontmap_data, w, h, m_char_data, 0, 255, font_height, 1024);

                m_font_height = font_height;
                m_font_scale = file.getFontScale(font_height);
            }

            m_font_map.setName("fontmap");
            m_font_map.setPixelFormat(BufferLayout({UND_UINT8}));
            m_font_map.setSize(w, h, 1);
            m_font_map.setFilteringMethod(UND_LINEAR, UND_LINEAR);

            m_font_map.setData((char*)fontmap_data, sizeof(unsigned char) * w * h);

            delete[] fontmap_data;
        }


        ////////////////////////////////////// getting data //////////////////////////////////////

        const Texture& Font::getFontMap() const{

            return m_font_map;
        }

        unsigned int Font::getFontHeight() const{

            return m_font_height;
        }


        float Font::getFontScale() const{
            /** for scaling the advance and offsets of characters
            * scale is linear, return matches the pixel height of getFontHeight */

            return m_font_scale;
        }

        //////////////////////////// building the data to render strings //////////////////////////

        void Font::buildString(std::vector<tools::FontCharData>& loadTo, const std::string& s) const{

            for(char c : s) {

                loadTo.push_back(m_char_data[(int) c]);
            }

        }


        float Font::buildString(float*& data, const std::string& s) const{
            /** @param data: (vec2)   8 bytes fontmap pos0,
            *                (vec2)   8 bytes fontmap pos1,
            *                (float)  4 bytes xadvance,
            *                (vec2)   8 bytes offset
            * total of 28 bytes per character which will be allocated
            * and need to be deleted manually */

            data = new float[s.length() * 7];

            float total_adv = 0;

            for(int i = 0; i < s.length(); i++) {

                data[i * 7 +  0] = m_char_data.at(s[i]).m_fm_pos0.x;
                data[i * 7 +  1] = m_char_data.at(s[i]).m_fm_pos0.y;

                data[i * 7 +  2] = m_char_data.at(s[i]).m_fm_pos1.x;
                data[i * 7 +  3] = m_char_data.at(s[i]).m_fm_pos1.y;

                data[i * 7 +  4] = total_adv;
                total_adv += m_char_data.at(s[i]).m_advance;

                data[i * 7 +  5] = m_char_data.at(s[i]).m_offset.x;
                data[i * 7 +  6] = m_char_data.at(s[i]).m_offset.y;

            }

            return total_adv;
        }

        float Font::getTotalAdvance(const std::string& s) const {
            /** @return the total advance (width of the text, unscaled) */

            float advance = 0;

            for(char c : s) {

                advance += m_char_data[(int) c].m_advance;
            }

            return advance;
        }

    } // graphics

} // undicht
