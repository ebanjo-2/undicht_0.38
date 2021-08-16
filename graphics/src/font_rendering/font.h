#ifndef FONT_H
#define FONT_H

#include <string>
#include <vector>

#include "core/texture.h"
#include "files/fonts/ttfont_file.h"

namespace undicht {

    namespace graphics {

        class Font {
                // a class containing a set of characters
                // with information about how to render them

                unsigned int m_font_height;
                float m_font_scale;

                Texture m_font_map;
                std::vector<tools::FontCharData> m_char_data;

            public:

                Font();
                Font(const std::string& file_name, unsigned int font_height);
                virtual ~Font();


            public:
                // setting data

                /** @param font_height: the height of the font in pixels */
                void loadFromFile(const std::string& file_name, unsigned int font_height);


            public:
                // getting data

                const Texture& getFontMap() const;

                unsigned int getFontHeight() const;

                /** for scaling the advance and offsets of characters
                * scale is linear, return matches the pixel height of getFontHeight */
                float getFontScale() const;

            public:
                // building the data to render strings

                void buildString(std::vector<tools::FontCharData>& loadTo, const std::string& s) const;

                /** @param data: (vec2)   8 bytes fontmap pos0,
                *                (vec2)   8 bytes fontmap pos1,
                *                (float)  4 bytes xadvance,
                *                (vec2)   8 bytes offset
                * total of 28 bytes per character which will be allocated
                * and need to be deleted manually
                * @return the total advance (width of the text, unscaled)*/
                float buildString(float*& data, const std::string& s) const;

                /** @return the total advance (width of the text, unscaled)*/
                float getTotalAdvance(const std::string& s) const;

        };

    } // graphics

} // undicht

#endif // FONT_H
