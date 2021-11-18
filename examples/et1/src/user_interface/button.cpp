#include <user_interface/button.h>

#include <iostream>

using namespace undicht;
using namespace graphics;
using namespace tools;

namespace et1 {


    Button::Button() {

    }

    Button::Button(Font& f, const std::string& caption) {

        setFont(f);
        setCaption(caption);

    }

    Button::Button(undicht::graphics::Font& f, const std::string& caption, glm::vec2 pos, glm::vec2 siz) {

        setFont(f);
        setCaption(caption);
        setPosition(pos);
        setSize(siz);
    }


    Button::~Button() {

    }

    bool Button::isPressed() {

        return m_pressed;
    }

    void Button::setBaseColor(const glm::vec4& color) {

        m_base_color = color;
    }

    void Button::setCaption(const std::string& caption) {

        m_caption = caption;

    }

    void Button::setFont(undicht::graphics::Font& f) {

        m_font = &f;
    }


    void Button::onDraw(undicht::graphics::FontRenderer& fr, undicht::graphics::VGRenderer& vr, const glm::vec2& parent_offset, const glm::vec2& parent_scale) const {

        if(!m_pressed) {

            vr.setColor(m_base_color);
        } else {

            vr.setColor(m_click_color);
        }

        fr.setFontColor(glm::vec3(1));

        glm::vec2 line_pos_0 = m_pos - glm::vec2(m_siz.x - m_siz.y, 0);
        glm::vec2 line_pos_1 = m_pos + glm::vec2(m_siz.x - m_siz.y, 0);

        line_pos_0 = line_pos_0 * parent_scale + parent_offset;
        line_pos_1 = line_pos_1 * parent_scale + parent_offset;
        float line_width = m_siz.y * parent_scale.y;

        vr.drawLine(line_pos_0, line_pos_1, line_width);
        fr.draw(*m_font, m_caption, line_pos_0 - glm::vec2(line_width, 0.3 * line_width));

    }

    bool Button::onMouseClick(const glm::vec2& pos, int button_id, int button_state) {

        if(button_state) {

            m_pressed = true;
        } else {

            m_pressed = false;
        }

        return true;
    }


} // et1
