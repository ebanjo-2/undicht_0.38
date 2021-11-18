#ifndef BUTTON_H
#define BUTTON_H

#include <string>

#include <user_interface/ui_element.h>

#include <font_rendering/font.h>
#include <font_rendering/font_renderer.h>
#include <vector_graphics/vg_renderer.h>

#include <glm/glm.hpp>

namespace et1 {

    class Button : public UIElement {

        protected:

            undicht::graphics::Font* m_font = 0;

            std::string m_caption;

            glm::vec4 m_base_color = glm::vec4(0.2, 0.2, 0.2, 1.0);
            glm::vec4 m_click_color = glm::vec4(0.7, 0.3, 0.1, 1.0);

            bool m_pressed = false;

        public:

            Button();
            Button(undicht::graphics::Font& f, const std::string& caption);
            Button(undicht::graphics::Font& f, const std::string& caption, glm::vec2 pos, glm::vec2 siz);

            virtual~ Button();

        public:

            virtual bool isPressed();

        public:

            virtual void setBaseColor(const glm::vec4& color);

            virtual void setCaption(const std::string& caption);

            virtual void setFont(undicht::graphics::Font& f);

        public:

            virtual void onDraw(undicht::graphics::FontRenderer& fr, undicht::graphics::VGRenderer& vr, const glm::vec2& parent_offset, const glm::vec2& parent_scale) const;

            /** returns true, if the click was handled by the element
            * (to notify parent elements on whether the child processed the info or if maybe it was meant for the parent)
            * pos will be relative to the element this gets called on */

            virtual bool onMouseClick(const glm::vec2& pos, int button_id, int button_state);
    };

} // et1

#endif // BUTTON_H
