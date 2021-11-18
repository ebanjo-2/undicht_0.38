#include <user_interface/ui_element.h>


namespace et1 {

    ///////////////////// functions to convert between parent and child space /////////////////////

    bool UIElement::isPartOfChildSpace(const glm::vec2& pos, const UIElement& c) {

        if((pos.x >= c.m_pos.x - c.m_siz.x) && (pos.x <= c.m_pos.x + c.m_siz.x))
            if((pos.y >= c.m_pos.y - c.m_siz.y) && (pos.y <= c.m_pos.y + c.m_siz.y))
                return true;

        return false;
    }

    glm::vec2 UIElement::toChildSpace(const glm::vec2& pos, const UIElement& child, const UIElement& parent) {

        glm::vec2 rel_pos = child.m_pos - pos;
        rel_pos.x /= parent.getAspectScale();
        rel_pos *= parent.m_siz / child.m_siz;
        rel_pos.x *= child.getAspectScale();

        return rel_pos;
    }

    //////////////////////////////////////// constructor / destructor //////////////////////////////////

    UIElement::UIElement() {


    }

    UIElement::~UIElement() {


    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // positions are either relative to the parent element
    // or, if there is none, to the display area (full screen / window / viewport)
    // positions go from -1 to 1 vertically
    // the visible horizontal range depends on the aspect ratio of the display / parent element
    // xmax = -1 * xmin = 1 * width / height
    // 0,0 is always the middle of the screen / window

    void UIElement::setPosition(const glm::vec2& pos) {

        m_pos = pos;

    }

    void UIElement::setSize(const glm::vec2& siz) {

        m_siz = siz;

    }

    const glm::vec2& UIElement::getPosition() const {

        return m_pos;
    }

    const glm::vec2& UIElement::getSize() const {

        return m_siz;
    }

    /** @return the width of the element divided by its height */
    float UIElement::getAspectScale() const {

        return m_siz.x / m_siz.y;
    }

    /////////////////////////////////////////////// managing hierarchy ///////////////////////////////////////////////

    void UIElement::addChild(UIElement* e) {
        /** @return the id with which the child will be accessible in the future */

        m_childs.push_back(e);
    }

    void UIElement::addChilds(std::vector<UIElement*> e) {

        for(UIElement* c : e)
            addChild(c);

    }


    void UIElement::remChild(UIElement* e) {

        for(int i = 0; i < m_childs.size(); i++) {

            if(e == m_childs[i]) {

                m_childs.erase(m_childs.begin() + i);
            }

        }

    }


    const std::vector<UIElement*>& UIElement::getChilds() const {

        return m_childs;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // functions that get called on events
    // if the event was not completely handled by the element
    // it should be passed further down to its children

    void UIElement::onDraw(undicht::graphics::FontRenderer& fr, undicht::graphics::VGRenderer& vr, const glm::vec2& parent_offset, const glm::vec2& parent_scale) const {

        for(const UIElement* c : m_childs)
            c->onDraw(fr, vr, parent_offset + m_pos * parent_scale, parent_scale * m_siz); // only if m_siz is (1,1), the parent_scale should not be changed

    }


    bool UIElement::onMouseClick(const glm::vec2& pos, int button_id, int button_state) {
        /** returns true, if the click was handled by the element
        * (to notify parent elements on whether the child processed the info or if maybe it was meant for the parent)
        * pos will be relative to the element this gets called on */


        for(UIElement* c : m_childs) {

            if(isPartOfChildSpace(pos, *c)) {

                glm::vec2 pos_for_child = toChildSpace(pos, *c, *this);

                if(c->onMouseClick(pos_for_child, button_id, button_state))
                    return true;
            }

        }

        // deriving classes may want to check here if they can make use of the event
        // .
        // .

        return false;
    }


} // et1
