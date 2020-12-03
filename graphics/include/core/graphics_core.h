#ifndef GRAPHICS_CORE_H
#define GRAPHICS_CORE_H

/* should be configured via cmake */
#define USE_GLFW
#define USE_GL33 

namespace undicht {

	namespace graphics {

		namespace interf {

			class Window;

			class GraphicsCore {
			public:

				GraphicsCore() = default;
				virtual ~GraphicsCore() = default;

				/** @return false, if the initialization was unsuccesfull */
				virtual bool init() = 0;

				/** @return false, if there are any errors unhandeled was unsuccesfull */
				virtual bool terminate() = 0;

				/** makes the window the one that gets drawn to
				* the first window created is by default the one that gets drawn to */
				virtual void makeDrawWindow(interf::Window& window) {}; // this function may take in the current implementations window, so no = 0 

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

#endif // USE_GL33

#endif // GRAPHICS_CORE_H