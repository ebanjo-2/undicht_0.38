#ifdef WINDOW_H // this file should only be included via include/window/window.h
#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H

#include <GLFW/glfw3.h>

namespace undicht {

	namespace graphics {

		namespace glfw {

			class Window : public interf::Window {

			public:

				static int s_open_windows;

				int m_width = 800;
				int m_height = 600;
				std::string m_title = "Window";

				bool m_cursor_visible = true;

				GLFWwindow* m_window = 0;

			public:

				static int getNumberOfOpenWindows();

				Window();
				Window(int width, int height, const std::string& title);
				virtual ~Window();

				/** opens the window
				* @return false, if the window could not be created */
				virtual bool open();

				virtual bool open(int width, int height, const std::string& title);

				virtual void close();

				virtual void update();

				/** \return whether a close was requested by the user */
				virtual bool shouldClose();

				virtual void setSize(int width, int height);

				virtual void getSize(int& width, int& height);

				virtual void setTitle(const std::string& title);

				virtual void setWindowMode(bool fullscreen, bool windowed);

				virtual void setCursorVisible(bool visible);

				virtual bool getCursorVisible();


			};

		} // glfw

	} // graphics

} // undicht


#endif // ! GLFW_WINDOW_H
#endif // WINDOW_H