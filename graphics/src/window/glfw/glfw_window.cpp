#include <core/graphics_core.h>
#ifdef USE_GLFW

#include <window/window.h>
#include <iostream>

namespace undicht {

    namespace graphics {

        namespace glfw {

            int Window::s_open_windows = 0;


            Window::Window() {

            }

            Window::Window(int width, int height, const std::string& title) {

                open(width, height, title);
            }

            Window::~Window() {
                // doing nothing
            }


            int Window::getNumberOfOpenWindows() {

                return s_open_windows;
            }

            bool Window::open() {
                /** opens the window
                * @return false, if the window could not be created */

                if (!(s_open_windows && GraphicsCore::isInitialized())) {
                    // first window, initializing the GraphicsCore
                    // might only init the window lib, since the graphics lib (i.e. glad) may need an open window

                    GraphicsCore::init();
                }

                m_window = glfwCreateWindow(m_width, m_height, m_title.data(), NULL, NULL); //glfwGetPrimaryMonitor() for fullscreen
                s_open_windows += 1;


                if (m_window == NULL) {
                    std::cout << "FAILED TO CREATE WINDOW" << "\n";
                    return false;
                }

                glfwMakeContextCurrent(m_window);



                if (!GraphicsCore::isInitialized()) {
                    // calling again to init the graphics lib

                    GraphicsCore::init();
                }

                return true;
            }

            bool Window::open(int width, int height, const std::string& title) {
                setSize(width, height);
                setTitle(title);
                return open();
            }

            void Window::close() {
                // to be done

                s_open_windows -= 1;

                if (!s_open_windows) {
                    // the last window just got closed
                    GraphicsCore::terminate();
                }

            }

            void Window::update() {
                getSize(m_width, m_height);
                glfwPollEvents();
                glfwSwapBuffers(m_window);
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

                    } else {
                        // window with default borders

                        glfwSetWindowMonitor(m_window, 0, 0, 0, m_width, m_height, 0);

                    }

                } else {

                    if (fullscreen) {
                        // true fullscreen mode

                        GLFWmonitor* monitor = glfwGetPrimaryMonitor();

                        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

                        glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

                    } else {
                        // borderless window, keeping the size
                        // not sure how to do this (after approx. 30 sec of research i couldnt find an easy result)
                    }

                }

            }

            void Window::setCursorVisible(bool visible) {

                m_cursor_visible = visible;

                if (visible) {
                    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                } else {
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
