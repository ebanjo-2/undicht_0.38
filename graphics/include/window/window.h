#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <core/graphics_core.h>

namespace undicht {

	namespace graphics {

		namespace interf {

			class Window {
				/** all window classes should implement these functions */
			public:

				Window() = default;
				virtual ~Window() = default;

				/** opens the window 
				* @return false, if the window could not be created */
				virtual bool open() = 0;

				virtual bool open(int width, int height, const std::string& title) = 0;

				virtual void close() = 0;

				virtual void update()= 0;

				/** \return whether a close was requested by the user */
				virtual bool shouldClose() = 0;

				virtual void setSize(int width, int height) = 0;

				virtual void getSize(int& width, int& height) = 0;

				virtual void setTitle(const std::string& title) = 0;

				virtual void setWindowMode(bool fullscreen, bool windowed) = 0;

				virtual void setCursorVisible(bool visible) = 0;

				virtual bool getCursorVisible() = 0;

			};

		} // interf

	} // graphics

} // undicht

#ifdef USE_GLFW

#include <window/glfw/glfw_window.h>

namespace undicht {
	namespace graphics {
		typedef glfw::Window Window;
	} // graphics
} // undicht

#endif // USE_GL33


#endif // WINDOW_H