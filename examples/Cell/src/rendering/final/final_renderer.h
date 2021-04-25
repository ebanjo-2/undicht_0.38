#ifndef FINAL_RENDERER_H
#define FINAL_RENDERER_H

#include <core/renderer.h>

#include <rendering/cell_gbuffer.h>

#include <material_register.h>

namespace cell {

	class FinalRenderer : public undicht::graphics::Renderer {

	public:

		undicht::graphics::Shader m_shader;
		undicht::graphics::VertexBuffer m_screen_quad;

	public:

		void drawFinal(const CellGBuffer& scene_cell_data, const MaterialRegister& materials);

		FinalRenderer();
		virtual ~FinalRenderer();

	};


} // cell

#endif // FINAL_RENDERER_H