// this file should only be included via include/core/graphics_core.h
#ifdef GRAPHICS_CORE_H

#ifndef GLFW_GL33_GRAPHICS_CORE_H
#define GLFW_GL33_GRAPHICS_CORE_H

#include <core/graphics_core.h>

namespace glfw_gl33 {

	class GraphicsCore : public interf::GraphicsCore {
	public:

		GraphicsCore();
		virtual ~GraphicsCore();

		/** @return false, if the initialization was unsuccesfull */
		virtual bool init();

		/** @return false, if there are any errors unhandeled was unsuccesfull */
		virtual bool terminate();

		/** makes the window the one that gets drawn to
		* the first window created is by default the one that gets drawn to */
		virtual void makeDrawWindow(interf::Window& window);

	};


}

#endif // GLFW_GL33_GRAPHICS_CORE_H

#endif // GRAPHICS_CORE_H