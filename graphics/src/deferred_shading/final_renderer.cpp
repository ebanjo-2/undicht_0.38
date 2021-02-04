#include "deferred_shading/final_renderer.h"
#include <files/file.h>

#include <string_tools.h>

namespace undicht {

	namespace graphics {

		using namespace tools;

#define FINAL_SHADER getFilePath(UND_CODE_SRC_FILE) + "final_shader.glsl"

		FinalRenderer::FinalRenderer() {

			init();
		}

		FinalRenderer::~FinalRenderer() {

			term();
		}

		void FinalRenderer::submitGBuffer(const GeometryBuffer& gbuffer) {

			m_shader.loadTexture(*gbuffer.getColorBuffer());
		}

		void FinalRenderer::submitLightMap(const Texture& light_map) {
			
			m_shader.loadTexture(light_map);
		}

		void FinalRenderer::drawFinal() {

			draw();
		}

		void FinalRenderer::init() {

			File final_shader_file;
			std::string shader_src;
			final_shader_file.open(FINAL_SHADER);

			m_shader.loadSource(final_shader_file.getAll(shader_src));

			submit(&m_shader);
			enableDepthTest(false);
			enableBackFaceCulling(false);

			// initializing the screen covering quad
			m_screen_quad.setLayout(BufferLayout({ UND_VEC3F, UND_VEC2F }));

			// quad vertices taken from https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/8.2.deferred_shading_volumes/deferred_shading_volumes.cpp
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
			};

			unsigned int quadIndices[] = {
				0,1,2,
				1,3,2
			};

			m_screen_quad.setData(quadVertices, sizeof(quadVertices));
			m_screen_quad.setIndexData(quadIndices, sizeof(quadIndices));

			submit(&m_screen_quad);
		}


		void FinalRenderer::term() {

		}

	} // graphics

} // undicht