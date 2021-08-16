#include "font_renderer.h"
#include <files/file.h>

#include <string_tools.h>
#include <types.h>
#include <glm/gtc/type_ptr.hpp>

#include <geometry/geometry.h>

#include <iostream>

using namespace undicht::tools;

#define FONT_SHADER getFilePath(UND_CODE_SRC_FILE) + "font_shader.glsl"

namespace undicht {

    namespace graphics {


        FontRenderer::FontRenderer() {

            init();
        }

        FontRenderer::~FontRenderer() {

        }

        void FontRenderer::init() {

            // initializing the shader
            File shader_file;
            std::string shader_src;
            shader_file.open(FONT_SHADER);

            m_shader.loadSource(shader_file.getAll(shader_src));
            submit(&m_shader);

            enableDepthTest(false);
            enableBackFaceCulling(false);
            // new text will always cover old text, but the spaces outside the characters wont
            enableBlending(true, UND_SRC_ALPHA, UND_ONE_MINUS_SRC_ALPHA);

            // initializing the screen covering quad
            m_quad.setLayout(BufferLayout({ UND_VEC2F, UND_VEC2F }));
            m_quad.setInstanceLayout(BufferLayout({ UND_VEC2F, UND_VEC2F, UND_FLOAT, UND_VEC2F }));

            std::vector<float> vertices;
            std::vector<int> indices;

            Geometry::genRectangle(glm::vec2(0, 0), glm::vec2(1, -1), vertices, indices);

            m_quad.setData(vertices);
            m_quad.setIndexData(indices);

            submit(&m_quad);

            // initializing the uniforms

            m_font_scale.setName("font_scale");
            m_text_pos.setName("text_pos");
            m_font_color.setName("font_color");


        }

        void FontRenderer::term() {

        }


        ///////////////////////////////////////////// drawing text /////////////////////////////////////////////


        void FontRenderer::setFontColor(glm::vec3 color) {

            m_font_color.setData(glm::value_ptr(color), UND_VEC3F);
            m_shader.loadUniform(m_font_color);

        }


        glm::vec2 FontRenderer::getFontScale(const Font& f, int font_pixel_height) {
            /** @return the scale for text in the given pixel height with the current viewport */

            int fm_w, fm_h;
            int vp_w, vp_h, vp_x, vp_y;

            f.getFontMap().getSize(fm_w, fm_h);
            getViewport(vp_w, vp_h, vp_x, vp_y);

            glm::vec2 fm_vp_scale(float(fm_w) / float(vp_w), float(fm_h) / float(vp_h));
            glm::vec2 font_scale = fm_vp_scale * (float(font_pixel_height) / float(f.getFontHeight())) * 2.0f;

            return font_scale;
        }

        void FontRenderer::draw(const Font& f, float* char_data, int char_count, glm::vec2 pos, glm::vec2 font_scale) {
            /** @param char_data: should contain the data for rendering a string
            * with a size of least 7 * sizeof(float) * char_count */

            m_font_scale.setData(glm::value_ptr(font_scale), UND_VEC2F);
            m_shader.loadUniform(m_font_scale);

            m_text_pos.setData(glm::value_ptr(pos), UND_VEC2F);
            m_shader.loadUniform(m_text_pos);

            m_shader.loadTexture(f.getFontMap());

            m_quad.setInstanceData(char_data, char_count * 7 * sizeof(float));
            submit(&m_quad);

            Renderer::draw(char_count);
        }


        void FontRenderer::draw(const Font& f, const std::string& text, glm::vec2 pos, int font_pixel_height) {
            /** @param pos: upper left corner of the baseline (characters such as j will go below)
            * @param font_pixel_height describes the maximum height of a line of text (capital letters + letters such as j)
            * about 1/3 of the font_pixel_height is below the baseline */

            float* char_data;
            float text_width = f.buildString(char_data, text); // scale is pixels on fontmap

            glm::vec2 font_scale = getFontScale(f, font_pixel_height);

            draw(f, char_data, text.length(), pos, font_scale);

            delete[] char_data;
        }



    } // graphics

} // undicht
