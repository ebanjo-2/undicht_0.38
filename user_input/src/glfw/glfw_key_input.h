#ifdef KEY_INPUT_H // this file should only be included via key_input.h

#include <key_ids.h>

namespace undicht {

	namespace user_input {

		namespace glfw {

			class KeyInput {

			public:

				graphics::Window* m_window = 0; // the window that this class detects the key input from

				static std::vector<int> s_pressed_keys;
				static std::vector<int> s_repeated_keys;
				static std::vector<int> s_released_keys;


			public:

				/** which windows input this class should register */
				virtual void setInputWindow(graphics::Window* window);

				/** clears the key presses registered,
				* example usage: call this when a new frame begins */
				virtual void clearKeyLists();

				/** stores the key presses in the order that they occurred in */
				virtual void getPressed(std::vector<int>& keys);

				virtual void getRepeated(std::vector<int>& keys);

				/** stores the key releases in the order that they occurred in */
				virtual void getReleased(std::vector<int>& keys);

				/** @return the state of the key (UND_KEY_PRESSED, ... (types.h) */
				virtual int getKeyState(int key);

			};

		} // glfw

	} // user_input

} // undicht

#endif