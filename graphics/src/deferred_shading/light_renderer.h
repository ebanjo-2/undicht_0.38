#include <core/renderer.h>
#include <core/frame_buffer.h>

#include "geometry_buffer.h"

#include <glm/glm.hpp>

#include <camera/perspective_camera_3d.h>

/** the renderer that draws the scene data to the geometry buffer */

namespace undicht {

	namespace graphics {


		class Light {

		public:

			float m_radius;

			glm::vec3 m_color;
			glm::vec3 m_position;

		public:

			Light(const glm::vec3& pos, const glm::vec3& color, float radius)
				: m_radius(radius), m_color(color), m_position(pos) {};

			virtual ~Light() {};


		};


		class LightRenderer : public Renderer {

		public:

			Shader m_shader;
			FrameBuffer m_light_buffer;
			Texture m_light_map;
			VertexBuffer m_light_model;

			// camera uniforms
			Uniform m_proj;
			Uniform m_view;
			Uniform m_viewrange;
			Uniform m_cam_pos;

			// light uniforms
			Uniform m_light_pos;
			Uniform m_light_color;
			Uniform m_light_radius;
			Uniform m_db_light_radius;

			Uniform m_viewport_size;

			void init();
			void term();

		public:


			virtual void setViewport(int width, int height, int offset_x = 0, int offset_y = 0);
			void loadCam(PerspectiveCamera3D& cam);
			void submitGBuffer(const GeometryBuffer& gbuffer);
			const Texture& getLightMap() const;


			void drawLight(const Light& light);

			LightRenderer();
			virtual ~LightRenderer();

		};


	} // graphics 

} // undicht