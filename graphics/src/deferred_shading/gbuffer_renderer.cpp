#include <deferred_shading/gbuffer_renderer.h>
#include <files/file.h>
#include <string_tools.h>

#include <glm/gtc/type_ptr.hpp>

namespace undicht {

	namespace graphics {

		using namespace tools;

#define GBUFFER_SHADER getFilePath(UND_CODE_SRC_FILE) + "gbuffer_shader.glsl"


		GBufferRenderer::GBufferRenderer() {
				
			init();
		}


		GBufferRenderer::~GBufferRenderer() {

			term();
		}

		void GBufferRenderer::setViewport(int width, int height, int offset_x, int offset_y) {

			m_viewport_width = width;
			m_viewport_height = height;
			m_viewport_offset_x = offset_x;
			m_viewport_offset_y = offset_y;

			m_gbuffer.setSize(width, height);
		}

		void GBufferRenderer::loadCam(PerspectiveCamera3D& cam) {

			// loading camera uniforms
			m_proj.setData(glm::value_ptr(cam.getCameraProjectionMatrix()), UND_MAT4F);
			m_shader.loadUniform(m_proj);

			m_view.setData(glm::value_ptr(cam.getViewMatrix()), UND_MAT4F);
			m_shader.loadUniform(m_view);

			m_cam_pos.setData(glm::value_ptr(cam.getWorldPosition()), UND_VEC3F);
			m_shader.loadUniform(m_cam_pos);

			float dvbvr = 1 / (cam.getViewRange() * 2);
			m_dvbvr.setData(&dvbvr, UND_FLOAT);
			m_shader.loadUniform(m_dvbvr);
		}

		const GeometryBuffer& GBufferRenderer::getGBuffer() const {

			return m_gbuffer;
		}


		void GBufferRenderer::init() {

			File gbuffer_shader_file(GBUFFER_SHADER);
			std::string shader_src;
			gbuffer_shader_file.getAll(shader_src);

			m_shader.loadSource(shader_src);

			m_gbuffer.setSize(800, 600);
			m_gbuffer.setDepthBuffer();
			m_gbuffer.setColorBuffer();
			m_gbuffer.setPositionBuffer();
			m_gbuffer.setNormalBuffer();

			submit(&m_shader);
			submit(&m_gbuffer);
			enableDepthTest(true);
			enableBackFaceCulling(true);

			// setting uniform names

			// camera uniforms
			m_proj.setName("proj");
			m_view.setName("view");
			m_dvbvr.setName("dvbvr");
			m_cam_pos.setName("cam_pos");

		}


		void GBufferRenderer::term() {

		}

	}// graphics

} // undicht

