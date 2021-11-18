#include <user_interface/ui_base.h>

namespace et1 {

    using namespace undicht;
    using namespace user_input;

    UIBase::UIBase(float scale, float aspect_ratio) {
        // aspect ratio is width / height

        m_aspect_ratio = aspect_ratio;
        m_scale = scale;

        m_siz = glm::vec2(1 / scale, 1 / scale);
        m_pos = glm::vec2(0,0);

    }

    UIBase::~UIBase() {

    }

    void UIBase::handleKeyInput(const undicht::user_input::KeyInput& k) {

    }

    void UIBase::handleMouseInput(const undicht::user_input::MouseInput& m) {

        float x,y;
        m.getCursorPosition(x,y);

        onMouseClick(glm::vec2(x * m_aspect_ratio,y) * m_scale, UND_MOUSE_1, m.getButtonState(UND_MOUSE_1));
    }


} // et1
