#include <core.h>
#ifdef USE_GLFW

#include <mouse_input.h>
#include <types.h>
#include <event_logger.h>

#include <GLFW/glfw3.h>

#include <iostream>

namespace undicht {

	using namespace tools;

	namespace user_input {

		namespace glfw {

			std::vector<int> MouseInput::s_pressed_buttons;
			std::vector<int> MouseInput::s_repeated_buttons;
			std::vector<int> MouseInput::s_released_buttons;

			int MouseInput::s_cursor_offset_x = 0;
			int MouseInput::s_cursor_offset_y = 0;
			int MouseInput::s_cursor_x = 0;
			int MouseInput::s_cursor_y = 0;
			int MouseInput::s_cursor_last_x = 0;
			int MouseInput::s_cursor_last_y = 0;
			bool first_mouse = true;


			///////////////////////////////////////////////// glfw callback functions /////////////////////////////////////////////////

			void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

				MouseInput::s_cursor_x = xpos;
				MouseInput::s_cursor_y = ypos;

				if (first_mouse) {
					// the first time a position is known
					MouseInput::s_cursor_last_x = MouseInput::s_cursor_x;
					MouseInput::s_cursor_last_y = MouseInput::s_cursor_y;
					first_mouse = false;
				}

			}


			void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {


				if (action = GLFW_PRESS) {

					MouseInput::s_pressed_buttons.push_back(button);
				}

				if (action = GLFW_REPEAT) {

					MouseInput::s_repeated_buttons.push_back(button);
				}

				if (action = GLFW_RELEASE) {

					MouseInput::s_released_buttons.push_back(button);
				}

			}


			/////////////////////////////////////////////// MouseInput functions //////////////////////////////////////////////////////

			void MouseInput::setInputWindow(graphics::Window* window) {

				m_window = window;

				glfwSetCursorPosCallback(m_window->m_window, mouse_callback);
				glfwSetMouseButtonCallback(m_window->m_window, mouse_button_callback);

			}

			////////////////////////////////////////////// Mouse Button Input //////////////////////////////////////////////


			void MouseInput::clearButtonLists() {
				/** clears the button presses registered,
				* example usage: call this when a new frame begins */

				s_pressed_buttons.clear();
				s_repeated_buttons.clear();
				s_released_buttons.clear();

			}

			void MouseInput::getPressed(std::vector<int>& keys) const {
				/** stores the button presses in the order that they occurred in */

				keys = s_pressed_buttons;
			}

			void MouseInput::getReleased(std::vector<int>& keys) const {
				/** stores the button releases in the order that they occurred in */

				keys = s_released_buttons;
			}

			int MouseInput::getButtonState(int button_id) const {
				/** @return the state (UND_KEY_PRESSED, UND_KEY_RELEASED ...) of the mouse button */

				return glfwGetMouseButton(m_window->m_window, button_id);
			}

			////////////////////////////////////////////////// Cursor Input //////////////////////////////////////////////

			void MouseInput::getCursorPosition(int& pos_x, int& pos_y) const {

				pos_x = s_cursor_x;
				pos_y = s_cursor_y;
			}

			void MouseInput::getCursorPosition(float& pos_x, float& pos_y) const {

				pos_x = float(s_cursor_x) / m_window->m_width * 2 - 1;
				pos_y = float(s_cursor_y) / m_window->m_height * -2 + 1;
			}


			void MouseInput::getCursorOffset(int& offset_x, int& offset_y) const {
				/** the distance moved by the cursor between the last two calls to updateCursorOffset() */

				offset_x = s_cursor_offset_x;
				offset_y = s_cursor_offset_y;
			}

			void MouseInput::getCursorOffset(float& offset_x, float& offset_y) const {
				/** the distance moved by the cursor between the last two calls to updateCursorOffset() */

				offset_x = float(s_cursor_offset_x) / m_window->m_width * 2;
				offset_y = float(s_cursor_offset_y) / m_window->m_height * -2;
			}

			void MouseInput::updateCursorOffset() {
				/** calculates the distance moved by the cursor
				* since the last call to this function */

				s_cursor_offset_x = s_cursor_x - s_cursor_last_x;
				s_cursor_offset_y = s_cursor_y - s_cursor_last_y;

				s_cursor_last_x = s_cursor_x;
				s_cursor_last_y = s_cursor_y;

			}


		} // glfw

	} // user_input

} // undicht


#endif // USE_GLFW
