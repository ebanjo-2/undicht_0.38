#include <core/renderer.h>
#include <deferred_shading/geometry_buffer.h>

namespace undicht {

	namespace graphics {
		/** the renderer that draws the scene data to the geometry buffer */

		class FinalRenderer : public Renderer {

		protected:

			Shader m_shader;
			VertexBuffer m_screen_quad;

			void init();
			void term();

		public:

			void submitGBuffer(const GeometryBuffer& gbuffer);
			void submitLightMap(const Texture& light_map);

			void drawFinal();


			FinalRenderer();
			virtual ~FinalRenderer();

		};

	} // graphics

} // undicht