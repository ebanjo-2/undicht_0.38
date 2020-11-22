#include <core/graphics_core.h>
#ifdef USE_GLFW
#ifdef USE_GL33

#include <GLFW/glfw3.h>


namespace undicht {

	namespace graphics {

		namespace glfw_gl33 {

			GraphicsCore::GraphicsCore() {


			}

			GraphicsCore::~GraphicsCore() {


			}

			bool GraphicsCore::init() {
				/** @return false, if the initialization was unsuccesfull */

				glfwInit();

				return true;
			}

			bool GraphicsCore::terminate() {
				/** @return false, if there are any errors unhandeled was unsuccesfull */

				glfwTerminate();

				return true;
			}


			void GraphicsCore::makeDrawWindow(interf::Window& window) {
				/** makes the window the one that gets drawn to
				* the first window created is by default the one that gets drawn to */

			}

		} // glfw_gl33

	} // graphics

} // undicht

#endif // USE_GL33

#endif // USE_GLFW