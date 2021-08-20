#include "player/player.h"

#include <types.h>

#define WALK_SPEED 0.1f
#define RUN_SPEED 1.0f


namespace cell {

    using namespace undicht;
    using namespace user_input;
    using namespace tools;

    Player::Player() {

		setViewRange(0.1, 10000);
    }

    Player::Player(const glm::vec3& pos) {

		setViewRange(0.1, 10000);
        setPosition(pos);

    }

    Player::~Player() {

    }


    //////////////////////////////////// user input handling ////////////////////////////////////

    void Player::loadKeyInput(const undicht::user_input::KeyInput& input) {

        if (input.getKeyState(UND_KEY_LSHIFT)) {

            m_speed = WALK_SPEED;
        } else {

            m_speed = RUN_SPEED;
        }

        if (input.getKeyState(UND_KEY_W)) {

            addTranslation(-1 * m_speed * getViewDirection());

        }

        if (input.getKeyState(UND_KEY_S)) {

            addTranslation(1 * m_speed  * getViewDirection());
        }

        if (input.getKeyState(UND_KEY_A)) {

            addTranslation(-1 * m_speed * getRightDirection());
        }

        if (input.getKeyState(UND_KEY_D)) {

            addTranslation(1 * m_speed  * getRightDirection());
        }
    }


    void Player::loadMouseInput(const undicht::user_input::MouseInput& input) {

        float offsetx, offsety;

        input.getCursorOffset(offsetx, offsety);

        m_yaw -= offsetx * 40;
        m_pitch += offsety * 40;

        setAxesRotation({ 0, m_pitch, m_yaw }, { UND_Z_AXIS, UND_X_AXIS, UND_Y_AXIS });
    }

} // cell
