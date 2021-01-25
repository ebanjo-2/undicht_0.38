#ifndef GRAPHICS_CORE_H
#define GRAPHICS_CORE_H

#include "core.h"

namespace undicht {

	namespace graphics {

		namespace interf {

			class GraphicsCore {
			public:

				/** @return false, if the initialization was unsuccesfull */				
				static bool init();

				/** @return if the graphics core was initialized successfully and didnt get terminated, this will return true  */
				static bool isInitialized();

				/** @return false, if there are any errors unhandeled was unsuccesfull */
				static bool terminate();

				/** makes the window the one that gets drawn to
				* the first window created is by default the one that gets drawn to */
				static void makeDrawWindow(void* window);

			};

		} // interf

	} // graphics

} // undicht

#if defined(USE_GLFW) && defined(USE_GL33)

#include <core/glfw_gl33/glfw_gl33_graphics_core.h>

namespace undicht {
	namespace graphics {
		typedef glfw_gl33::GraphicsCore GraphicsCore;
	} // graphics
} // undicht

#endif // USE_GL33 && USE_GLFW

#endif // GRAPHICS_CORE_H