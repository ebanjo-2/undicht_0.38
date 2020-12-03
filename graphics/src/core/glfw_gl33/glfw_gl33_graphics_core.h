// this file should only be included via include/core/graphics_core.h
#ifdef GRAPHICS_CORE_H

#ifndef GLFW_GL33_GRAPHICS_CORE_H
#define GLFW_GL33_GRAPHICS_CORE_H

namespace undicht {

	namespace graphics {

		namespace glfw_gl33 {

			class Window;

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
				virtual void makeDrawWindow(Window& window);

			};

		} // glfw_gl33

	} // graphics

} // undicht

#endif // GLFW_GL33_GRAPHICS_CORE_H

#endif // GRAPHICS_CORE_H