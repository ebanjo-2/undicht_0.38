#ifndef KEY_INPUT_H
#define KEY_INPUT_H


#include <core.h>
#include <window/window.h>
#include "key_ids.h"
#include <types.h>

#include <vector>

namespace undicht {

	namespace user_input {

		namespace interf {


			class KeyInput {


			public:

				/** which windows input this class should register */
				virtual void setInputWindow(graphics::Window* window) = 0;

				/** clears the key presses registered,
				* example usage: call this when a new frame begins */
				virtual void clearKeyLists() = 0;

				/** stores the key presses in the order that they occurred in */
				virtual void getPressed(std::vector<int>& keys) = 0;

				/** stores the key releases in the order that they occurred in */
				virtual void getReleased(std::vector<int>& keys) = 0;

				/** @return the state of the key (UND_KEY_PRESSED, ... (types.h) */
				virtual int getKeyState(int key) = 0;

			};


		} // interf


	} // user input

} // undicht


#if defined(USE_GLFW)

#include "glfw/glfw_key_input.h"

namespace undicht {
	namespace user_input {
		typedef glfw::KeyInput KeyInput;
	} // user_input
} // undicht

#endif // USE_GLFW


#endif // !KEY_INPUT_H