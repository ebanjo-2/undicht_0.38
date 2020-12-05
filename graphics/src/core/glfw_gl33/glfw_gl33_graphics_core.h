// this file should only be included via include/core/graphics_core.h
#ifdef GRAPHICS_CORE_H

#ifndef GLFW_GL33_GRAPHICS_CORE_H
#define GLFW_GL33_GRAPHICS_CORE_H

namespace undicht {

	namespace graphics {

		namespace glfw_gl33 {

			class GraphicsCore : public interf::GraphicsCore {

			public:

				static bool s_glfw_initialized;
				static bool s_glad_initialized;

			public:

				/** @return false, if the initialization was unsuccesfull */
				static bool init();

				/** @return if the graphics core was initialized successfully and didnt get terminated, this will return true  */
				static bool isInitialized();

				/** @return false, if there are any errors unhandeled was unsuccesfull */
				static bool terminate();

				/** makes the window the one that gets drawn to
				* the first window created is by default the one that gets drawn to */
				static void makeDrawWindow(void* window); // window.h includes this file, which makes things a little complicated

			};

		} // glfw_gl33

	} // graphics

} // undicht

#endif // GLFW_GL33_GRAPHICS_CORE_H

#endif // GRAPHICS_CORE_H