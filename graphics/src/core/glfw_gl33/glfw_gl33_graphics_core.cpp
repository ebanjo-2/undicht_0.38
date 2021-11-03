#include <core/graphics_core.h>
#ifdef USE_GLFW
#ifdef USE_GL33

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window/window.h"
#include "note.h"
#include "event_logger.h"

using namespace undicht::tools;

namespace undicht {

	namespace graphics {

		namespace glfw_gl33 {

			bool GraphicsCore::s_glfw_initialized = false;
			bool GraphicsCore::s_glad_initialized = false;


			bool GraphicsCore::init() {
				/** @return false, if the initialization was unsuccesfull */

				// initializing glfw
				if (!s_glfw_initialized) {
					glfwInit();
					s_glfw_initialized = true;

                    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				}


				// initializing glad
				if ((!s_glad_initialized) && Window::getNumberOfOpenWindows()) {
					// glad wont init without a window opened before
					if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { //loading all opengl functions
						EventLogger::storeNote(Note(UND_ERROR, "Failed to initialize GLAD", UND_CODE_ORIGIN));

						return false;
					}
					s_glad_initialized = true;
				}

				// adding opengl errors to the event classes collection of errors
				Note::setStaticMessage("ERROR 1280: GL_INVALID_ENUM", 1280);
				Note::setStaticMessage("ERROR 1281: GL_INVALID_VALUE", 1281);
				Note::setStaticMessage("ERROR 1282: GL_INVALID_OPERATION", 1282);
				Note::setStaticMessage("ERROR 1283: GL_STACK_OVERFLOW", 1283);
				Note::setStaticMessage("ERROR 1284: GL_STACK_UNDERFLOW", 1284);
				Note::setStaticMessage("ERROR 1285: GL_OUT_OF_MEMORY", 1285);
				Note::setStaticMessage("ERROR 1286: GL_INVALID_FRAMEBUFFER_OPERATION", 1286);



				return true;
			}


			/** @return if the graphics core was initialized successfully and didnt get terminated, this will return true  */
			bool GraphicsCore::isInitialized() {

				return s_glad_initialized && s_glfw_initialized;
			 }



			bool GraphicsCore::terminate() {
				/** @return false, if there are any errors unhandeled was unsuccesfull */

				glfwTerminate();

				s_glad_initialized = false;
				s_glfw_initialized = false;

				return true;
			}


			void GraphicsCore::makeDrawWindow(void* window) {
				/** makes the window the one that gets drawn to
				* the first window created is by default the one that gets drawn to */

			}

		} // glfw_gl33

	} // graphics

} // undicht

#endif // USE_GL33

#endif // USE_GLFW
