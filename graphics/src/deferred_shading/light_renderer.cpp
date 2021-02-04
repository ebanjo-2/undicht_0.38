#include <deferred_shading/light_renderer.h>
#include <files/file.h>
#include <files/model_loading/collada/collada_file.h>

#include <iostream>

#include <string_tools.h>

#include <glm/gtc/type_ptr.hpp>

namespace undicht {

	namespace graphics {

		using namespace tools;


#define LIGHT_SHADER getFilePath(UND_CODE_SRC_FILE) + "light_shader.glsl"
#define LIGHT_MODEL getFilePath(UND_CODE_SRC_FILE) + "light.dae"

		LightRenderer::LightRenderer() {

			init();
		}

		LightRenderer::~LightRenderer() {

			term();
		}


		void LightRenderer::setViewport(int width, int height, int offset_x, int offset_y) {

			m_viewport_width = width;
			m_viewport_height = height;
			m_viewport_offset_x = offset_x;
			m_viewport_offset_y = offset_y;

			m_light_buffer.setSize(width, height);
		}

		void LightRenderer::loadCam(PerspectiveCamera3D& cam) {

			// loading camera uniforms
			m_proj.setData(glm::value_ptr(cam.getCameraProjectionMatrix()), UND_MAT4F);
			m_shader.loadUniform(m_proj);

			m_view.setData(glm::value_ptr(cam.getViewMatrix()), UND_MAT4F);
			m_shader.loadUniform(m_view);

			m_cam_pos.setData(glm::value_ptr(cam.getWorldPosition()), UND_VEC3F);
			m_shader.loadUniform(m_cam_pos);

			float viewrange = cam.getViewRange();
			m_viewrange.setData(&viewrange, UND_FLOAT);
			m_shader.loadUniform(m_viewrange);
		}


		void LightRenderer::submitGBuffer(const GeometryBuffer& gbuffer) {

			Texture* depth = (Texture *)(gbuffer.getDepthBuffer());

			m_light_buffer.addAttachment(*depth, UND_DEPTH_ATTACHMENT_READ_AND_WRITE);
			m_shader.loadTexture(*gbuffer.getPositionBuffer());
			m_shader.loadTexture(*gbuffer.getNormalBuffer());

		}

		const Texture& LightRenderer::getLightMap() const {

			return m_light_map;
		}


		void LightRenderer::drawLight(const Light& light) {

			m_light_pos.setData(glm::value_ptr(light.m_position), UND_VEC3F);
			m_shader.loadUniform(m_light_pos);

			m_light_color.setData(glm::value_ptr(light.m_color), UND_VEC3F);
			m_shader.loadUniform(m_light_color);

			m_light_radius.setData(&light.m_radius, UND_FLOAT);
			m_shader.loadUniform(m_light_radius);

			float db_lr = 1.0f / light.m_radius;
			m_db_light_radius.setData(&db_lr, UND_FLOAT);
			m_shader.loadUniform(m_db_light_radius);

			float viewport[] = { 1.0f / m_light_buffer.m_width, 1.0f / m_light_buffer.m_height };
			m_viewport_size.setData(viewport, UND_VEC2F);
			m_shader.loadUniform(m_viewport_size);

			draw();

		}

		void LightRenderer::init() {

			File light_shader_file(LIGHT_SHADER);
			std::string shader_src;
			light_shader_file.getAll(shader_src);

			m_shader.loadSource(shader_src);

			m_light_map.setPixelFormat(BufferLayout({ UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR, UND_UNSIGNED_CHAR }));
			m_light_map.setFilteringMethod(UND_NEAREST, UND_NEAREST);
			m_light_map.setWrappingMethod(UND_REPEAT);
			m_light_map.setName("light_map");

			m_light_buffer.setSize(800, 600);
			m_light_buffer.addAttachment(m_light_map, UND_COLOR_ATTACHMENT, 0);


			submit(&m_shader);
			submit(&m_light_buffer);
			enableDepthTest(true, false, UND_GREATER);
			enableBackFaceCulling(true, UND_FRONT_FACE);
			enableBlending(true, UND_SRC_ALPHA, UND_ONE_MINUS_SRC_ALPHA);

			// loading the light sphere model
			ColladaFile model_loader;
			model_loader.open(LIGHT_MODEL);

			MeshData data;
			model_loader.getMesh(data);

			m_light_model.setData(data.vertices);
			m_light_model.setLayout(data.vertex_layout);

			submit(&m_light_model);

			// setting uniform names

			// camera uniforms
			m_proj.setName("proj");
			m_view.setName("view");
			m_viewrange.setName("viewrange");
			m_cam_pos.setName("cam_pos");

			// light uniforms
			m_light_pos.setName("light_origin");
			m_light_radius.setName("light_radius");
			m_light_color.setName("light_color");
			m_db_light_radius.setName("db_light_radius");
			m_viewport_size.setName("viewport_size");
		}


		void LightRenderer::term() {

		}


	} // graphics

} // undicht