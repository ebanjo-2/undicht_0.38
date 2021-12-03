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

    //////////////////////////////////// reacting to the environment ////////////////////////////////////


    void Player::checkCollision(World& w, float delta_time) {

        // applying the force
        m_vel += m_force * delta_time / m_mass;
        m_force = glm::vec3(0,0,0); // reset for next frame

        // meaningfull positions to check
        glm::vec3 feet_pos = getPosition() - glm::vec3(0,18,0);

        glm::vec3 next_feet_posx = feet_pos;
        next_feet_posx.x += 1.0f * m_vel.x / std::fabs(m_vel.x) - 0.5f;
        next_feet_posx.y += 1.0f;

        glm::vec3 next_feet_posz = feet_pos;
        next_feet_posz.z += 1.0f * m_vel.z / std::fabs(m_vel.z) - 0.5f;
        next_feet_posz.y += 1.0f;

        // standing on solid ground
        if(w.getCellAt(glm::ivec3(feet_pos)).m_material == VOID_CELL) {

            m_force += glm::vec3(0,-9.81f * m_mass, 0); // gravity
            m_falling = true;

        } else {

            m_vel.y = std::max(0.0f, m_vel.y);
            m_falling = false;
        }

        // running into the wall
        // in x direction
        if(w.getCellAt(glm::ivec3(next_feet_posx)).m_material != VOID_CELL) {

            m_vel.x = 0.0f;

        }

        // in z direction
        if(w.getCellAt(glm::ivec3(next_feet_posz)).m_material != VOID_CELL) {

            m_vel.z = 0.0f;

        }

    }

    void Player::processMovement(float delta_time) {

        // applying the velocity
        setPosition(getPosition() + m_vel * delta_time);

    }


    //////////////////////////////////// user input handling ////////////////////////////////////

    void Player::loadKeyInput(const undicht::user_input::KeyInput& input) {

        float walk_force = 1000;

        if (input.getKeyState(UND_KEY_LSHIFT)) {

            walk_force = 3000;
        } else {

            walk_force = 3000;
        }

        if (input.getKeyState(UND_KEY_W)) {

            m_force += -1 * walk_force * getViewDirection() * glm::vec3(1,0,1);
        }

        if (input.getKeyState(UND_KEY_S)) {

            m_force += 1 * walk_force * getViewDirection() * glm::vec3(1,0,1);
        }

        if (input.getKeyState(UND_KEY_A)) {

            m_force += -1 * walk_force * getRightDirection();
        }

        if (input.getKeyState(UND_KEY_D)) {

            m_force += 1 * walk_force * getRightDirection();
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
