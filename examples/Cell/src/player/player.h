#ifndef PLAYER_H
#define PLAYER_H

#include <camera/perspective_camera_3d.h>

#include <glm/glm.hpp>

#include <key_input.h>
#include <mouse_input.h>
#include <world/world.h>

namespace cell {

    class Player : public undicht::graphics::PerspectiveCamera3D {

            float m_pitch = 0;
            float m_yaw = 0;


        public:
            // physics

            float m_mass = 80.0f;

            glm::vec3 m_force; // will be applied to the velocity next time processMovement is called
            glm::vec3 m_vel;

            float m_falling = false;


        public:

            Player();
            Player(const glm::vec3& pos);
            virtual ~Player();

        public:
            // physics

            void checkCollision(World& w, float delta_time);

            void processMovement(float delta_time);

        public:
            // user input handling

            void loadKeyInput(const undicht::user_input::KeyInput& input);
            void loadMouseInput(const undicht::user_input::MouseInput& input);

    };


} // cell

#endif
