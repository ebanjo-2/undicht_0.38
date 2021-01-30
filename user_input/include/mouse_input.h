#ifndef MOUSE_INPUT_H
#define MOUSE_INPUT_H


#include <core.h>
#include <window/window.h>
#include "key_ids.h"
#include <types.h>

#include <vector>

namespace undicht {

	namespace user_input {

		namespace interf {


			class MouseInput {

			public:

				/** which windows input this class should register */
				virtual void setInputWindow(graphics::Window* window) = 0;

				/** clears the button presses registered,
				* example usage: call this when a new frame begins */
				virtual void clearButtonLists() = 0;

				/** stores the button presses in the order that they occurred in */
				virtual void getPressed(std::vector<int>& keys) = 0;

				/** stores the button releases in the order that they occurred in */
				virtual void getReleased(std::vector<int>& keys) = 0;

				/** @return the state of the button (UND_KEY_PRESSED, ... (types.h) */
				virtual int getButtonState(int key) = 0;

				/** the pixel position of the cursor gets stored */
				virtual void getCursorPosition(int& pos_x, int& pos_y) const = 0;

				/** the relative position (0.0f to 1.0f) gets stored */
				virtual void getCursorPosition(float& pos_x, float& pos_y) const = 0;

				/** the distance moved by the cursor between the last two calls to updateCursorOffset() */
				virtual void getCursorOffset(int& offset_x, int& offset_y) const = 0;

				/** the distance moved by the cursor between the last two calls to updateCursorOffset()
				* in the relative coordinate system */
				virtual void getCursorOffset(float& offset_x, float& offset_y) const = 0;

				/** calculates the distance moved by the cursor
				* since the last call to this function */
				virtual void updateCursorOffset() = 0;
			};


		} // interf


	} // user input

} // undicht


#if defined(USE_GLFW)

#include "glfw/glfw_mouse_input.h"

namespace undicht {
	namespace user_input {
		typedef glfw::MouseInput MouseInput;
	} // user_input
} // undicht

#endif // USE_GLFW


#endif // !MOUSE_INPUT_H