#ifdef MOUSE_INPUT_H // this file should only be included via key_input.h

#include <key_ids.h>
#include <array>

namespace undicht {

	namespace user_input {

		namespace glfw {

			class MouseInput {

			public:

				graphics::Window* m_window = 0; // the window that this class detects the key input from

				static std::array<int, 8> s_mouse_buttons;
				static int s_cursor_offset_x;
				static int s_cursor_offset_y;
				static int s_cursor_x;
				static int s_cursor_y;
				static int s_cursor_last_x;
				static int s_cursor_last_y;

				// to track the changed buttons
				static std::vector<int> s_pressed_buttons;
				static std::vector<int> s_repeated_buttons;
				static std::vector<int> s_released_buttons;

			public:

				/** which windows input this class should register */
				virtual void setInputWindow(graphics::Window* window);

				/** clears the button presses registered,
				* example usage: call this when a new frame begins */
				virtual void clearButtonLists();

				/** stores the button presses in the order that they occurred in */
				virtual void getPressed(std::vector<int>& keys);

				/** stores the button releases in the order that they occurred in */
				virtual void getReleased(std::vector<int>& keys);

				/** @return the state of the button (UND_KEY_PRESSED, ... (types.h) */
				virtual int getButtonState(int key) const;

				/** the pixel position of the cursor gets stored */
				virtual void getCursorPosition(int& pos_x, int& pos_y) const;

				/** the relative position (0.0f to 1.0f) gets stored */
				virtual void getCursorPosition(float& pos_x, float& pos_y) const;

				/** the distance moved by the cursor between the last two calls to updateCursorOffset() */
				virtual void getCursorOffset(int& offset_x, int& offset_y) const;

				/** the distance moved by the cursor between the last two calls to updateCursorOffset()
				* in the relative coordinate system */
				virtual void getCursorOffset(float& offset_x, float& offset_y) const;

				/** calculates the distance moved by the cursor
				* since the last call to this function */
				virtual void updateCursorOffset();
			};

		} // glfw

	} // user_input

} // undicht

#endif