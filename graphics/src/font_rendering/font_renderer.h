#ifndef FONT_RENDERER_H
#define FONT_RENDERER_H

#include <core/renderer.h>
#include "font.h"

namespace undicht {

    namespace graphics {

        class FontRenderer : public Renderer {

            public:

                Shader m_shader;
                VertexBuffer m_quad;

                Uniform m_font_scale;
                Uniform m_text_pos;
                Uniform m_font_color;

                Font* m_font = 0;

            public:

                FontRenderer();
                virtual ~FontRenderer();

                void init();
                void term();

            public:
                // drawing text

                void setFontColor(glm::vec3 color);

                /** @return the scale for text in the given pixel height with the current viewport */
                glm::vec2 getFontScale(const Font& f, int font_pixel_height);

                /** @param char_data: should contain the data for rendering a string
                * with a size of least 7 * sizeof(float) * char_count */
                void draw(const Font&f, float* char_data, int char_count, glm::vec2 pos, glm::vec2 font_scale);

                /** @param pos: upper left corner of the baseline (characters such as j will go below)
                * @param font_pixel_height describes the maximum height of a line of text (capital letters + letters such as j)
                * about 1/3 of the font_pixel_height is below the baseline */
                void draw(const Font& f, const std::string& text, glm::vec2 pos, int font_pixel_height = 20);

        };

    } // graphics

} // undicht

#endif // FONT_RENDERER_H
