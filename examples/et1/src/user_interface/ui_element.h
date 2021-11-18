#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include <glm/glm.hpp>
#include <vector>

#include <font_rendering/font_renderer.h>
#include <vector_graphics/vg_renderer.h>

namespace et1 {


    class UIElement {
        /** this class should be the parent to all ui elements such as buttons, labels, dialog panels, ...
        * it defines a hierarchy of ui elements, where ui elements can have child elements
        * also it adds functions through which events such as user input are distributed
        * and functions that give ui elements the opportunity to be displayed */

        public:
            // functions to convert between parent and child space

            static bool isPartOfChildSpace(const glm::vec2& pos, const UIElement& c);

            static glm::vec2 toChildSpace(const glm::vec2& pos, const UIElement& child, const UIElement& parent);

        protected:
            // position / size

            glm::vec2 m_pos;
            glm::vec2 m_siz;

        protected:
            // hierarchy (childs to this element)
            // tell the parent when you delete children

            std::vector<UIElement*> m_childs;

        public:
            // constructor / destructor


            UIElement();
            virtual ~UIElement();

        public:
            // positions are either relative to the parent element
            // or, if there is none, to the display area (full screen / window / viewport)
            // positions go from -1 to 1 in both directions
            // 0,0 is always the middle of the screen / window / parent UIElement

            // the position is the middle of the element
            virtual void setPosition(const glm::vec2& pos);


            // size means the distance needed to get from the middle to the upper right corner
            // an element with y size of 1 fills the entire screen
            virtual void setSize(const glm::vec2& siz);

            virtual const glm::vec2& getPosition() const;

            virtual const glm::vec2& getSize() const;

            /** @return the width of the element divided by its height */
            virtual float getAspectScale() const;

        public:
            // managing hierarchy

            virtual void addChild(UIElement* e);

            virtual void addChilds(std::vector<UIElement*> e);

            virtual void remChild(UIElement* e);

            virtual const std::vector<UIElement*>& getChilds() const;

        public:
            // functions that get called on events
            // if the event was not completely handled by the element
            // it should be passed further down to its children

            virtual void onDraw(undicht::graphics::FontRenderer& fr, undicht::graphics::VGRenderer& vr, const glm::vec2& parent_offset = glm::vec2(0), const glm::vec2& parent_scale = glm::vec2(1)) const;

            /** returns true, if the click was handled by the element
            * (to notify parent elements on whether the child processed the info or if maybe it was meant for the parent)
            * pos will be relative to the element this gets called on */
            virtual bool onMouseClick(const glm::vec2& pos, int button_id, int button_state);


    };


} // et1

#endif // UI_ELEMENT_H
