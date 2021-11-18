#ifndef UI_BASE_H
#define UI_BASE_H

#include <user_interface/ui_element.h>

#include <mouse_input.h>
#include <key_input.h>

namespace et1 {

    class UIBase : public UIElement {
            // the parent ui element to all other ui elements
            // per default it fills the entire screen
            // takes in events and delivers them to its childs

        public:

            float m_scale = 1;
            float m_aspect_ratio = 1;


        public:


            /** @param scale: the inverse (1/scale) is applied to all child element positions and sizes
            * can be used to make the positions of the next layer of elements be pixel based
            * (set scale to WINDOW_HEIGHT / 2) */
            UIBase(float scale, float aspect_ratio);
            virtual ~UIBase();

            void handleKeyInput(const undicht::user_input::KeyInput& k);

            void handleMouseInput(const undicht::user_input::MouseInput& m);

    };


} // et1


#endif // UI_BASE_H
