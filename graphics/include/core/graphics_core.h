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
				virtual void makeDrawWindow(interf::Window& window) = 0;

			};

		} // interf

#ifdef USE_GLFW
#ifdef USE_GL33
#include <core/glfw_gl33/glfw_gl33_graphics_core.h>
		typedef glfw_gl33::GraphicsCore GraphicsCore;
#endif // USE_GLFW
#endif // USE_GL33

	} // graphics

} // undicht

#endif // GRAPHICS_CORE_H