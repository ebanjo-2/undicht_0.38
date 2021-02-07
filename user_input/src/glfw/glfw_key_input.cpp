#include <core.h>
#ifdef USE_GLFW

#include <key_input.h>
#include "glfw_key_ids.h"
#include <types.h>
#include <event_logger.h>

#include <GLFW/glfw3.h>

#include <iostream>

namespace undicht {

	using namespace tools;

	namespace user_input {

		namespace glfw {

			std::vector<int> KeyInput::s_pressed_keys;
			std::vector<int> KeyInput::s_repeated_keys;
			std::vector<int> KeyInput::s_released_keys;

			///////////////////////////////////////////////// glfw callback functions /////////////////////////////////////////////////

			void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

				if (action == GLFW_PRESS) {

					KeyInput::s_pressed_keys.push_back(key);
				}

				if (action == GLFW_REPEAT) {

					KeyInput::s_repeated_keys.push_back(key);
				}

				if (action == GLFW_RELEASE) {

					KeyInput::s_released_keys.push_back(key);
				}

			}

			///////////////////////////////////////////////// KeyInput implementation /////////////////////////////////////////////////


			void KeyInput::setInputWindow(graphics::Window* window) {
				/** which windows input this class should register */

				initKeyIds();
				glfwSetKeyCallback(window->m_window, key_callback);
				m_window = window;
			}


			void KeyInput::clearKeyLists() {
				/** clears the key presses registered,
				* example usage: call this when a new frame begins */

				s_pressed_keys.clear();
				s_repeated_keys.clear();
				s_released_keys.clear();
			}

			void KeyInput::getPressed(std::vector<int>& keys) {
				/** stores the key presses in the order that they occurred in */

				keys = s_pressed_keys;
			}

			void KeyInput::getRepeated(std::vector<int>& keys) {

				keys = s_repeated_keys;
			}

			void KeyInput::getReleased(std::vector<int>& keys) {
				/** stores the key releases in the order that they occurred in */

				keys = s_released_keys;
			}

			int KeyInput::getKeyState(int key) {
				/** @return the state of the key (UND_KEY_PRESSED, ... (types.h) */

				if (m_window) {

					switch (glfwGetKey(m_window->m_window, key)) {
					case GLFW_PRESS:
						return UND_KEY_PRESSED;
					case GLFW_REPEAT:
						return UND_KEY_REPEATED;
					case GLFW_RELEASE:
						return UND_KEY_RELEASED;
					}

				}
				else {
					EventLogger::storeNote(Note(UND_ERROR, "KeyInputWatcher has no window set", UND_CODE_ORIGIN));
					return UND_TYPE_UNAVAILABLE;
				}

			}



		} // glfw

	} // user_input

} // undicht


#endif // USE_GLFW
