#ifndef PLAYER_H
#define PLAYER_H

#include <camera/perspective_camera_3d.h>

#include <glm/glm.hpp>

#include <key_input.h>
#include <mouse_input.h>

namespace cell {

    class Player : public undicht::graphics::PerspectiveCamera3D {

            float m_pitch = 0;
            float m_yaw = 0;

            float m_speed = 0.1f;

        public:

            Player();
            Player(const glm::vec3& pos);
            virtual ~Player();

        public:
            // user input handling

            void loadKeyInput(const undicht::user_input::KeyInput& input);
            void loadMouseInput(const undicht::user_input::MouseInput& input);

    };


} // cell

#endif
