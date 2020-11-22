#include <core/graphics_core.h>
#ifdef USE_GLFW

#include <window/window.h>
#include <iostream>

namespace undicht {

	namespace graphics {

		namespace glfw {

			Window::Window() {

			}

			Window::~Window() {
				// doing nothing
			}


			bool Window::open() {
				/** opens the window
				* @return false, if the window could not be created */

				m_window = glfwCreateWindow(m_width, m_height, m_title.data(), NULL, NULL); //glfwGetPrimaryMonitor() for fullscreen

				if (m_window == NULL) {
					std::cout << "FAILED TO CREATE WINDOW" << "\n";
					return false;
				}

				glfwMakeContextCurrent(m_window);

				return true;
			}

			bool Window::open(int width, int height, const std::string& title) {
				setSize(width, height);
				setTitle(title);
				return open();
			}

			void Window::close() {
				// to be done
			}

			void Window::update() {
				getSize(m_width, m_height);
				glfwPollEvents();
			}

			bool Window::shouldClose() {
				/** @return whether a close was requested by the user */

				return glfwWindowShouldClose(m_window);
			}


			void Window::setSize(int width, int height) {

				m_width = width;
				m_height = height;

				if (m_window) {
					glfwSetWindowSize(m_window, width, height);
				} // to be done

			}

			void Window::getSize(int& width, int& height) {

				glfwGetWindowSize(m_window, &width, &height);

			}

			void Window::setTitle(const std::string& title) {

				m_title = title;

				if (m_window) {
					glfwSetWindowTitle(m_window, title.data());
				}

			}

			void Window::setWindowMode(bool fullscreen, bool windowed) {
				// still to be done

				if (windowed) {

					if (fullscreen) {
						// window without border the size of the screen

						GLFWmonitor* monitor = glfwGetPrimaryMonitor();

						const GLFWvidmode* mode = glfwGetVideoMode(monitor);

						glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

					}
					else {
						// window with default borders

						glfwSetWindowMonitor(m_window, 0, 0, 0, m_width, m_height, 0);

					}

				}
				else {

					if (fullscreen) {
						// true fullscreen mode

						GLFWmonitor* monitor = glfwGetPrimaryMonitor();

						const GLFWvidmode* mode = glfwGetVideoMode(monitor);

						glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

					}
					else {
						// borderless window, keeping the size
						// not sure how to do this (after approx. 30 sec of research i couldnt find an easy result)
					}

				}

			}

			void Window::setCursorVisible(bool visible) {

				m_cursor_visible = visible;

				if (visible) {
					glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}
				else {
					glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//cursor disappears
				}

			}

			bool Window::getCursorVisible() {

				return m_cursor_visible;
			}
		}

	} // graphics

} // undicht


#endif // USE_GLFW