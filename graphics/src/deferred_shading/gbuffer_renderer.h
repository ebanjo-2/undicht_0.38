#include <core/renderer.h>
#include <deferred_shading/geometry_buffer.h>

#include <camera/perspective_camera_3d.h>


/** the renderer that draws the scene data to the geometry buffer */


namespace undicht {

	namespace graphics {

		class GBufferRenderer : public Renderer {

		public:

			Shader m_shader;
			GeometryBuffer m_gbuffer;

			// camera uniforms
			Uniform m_proj;
			Uniform m_view;
			Uniform m_dvbvr; // 1 divided by double the viewrange (used to convert position data into something that can be stored in a texture)
			Uniform m_cam_pos;

			void init();
			void term();

		public:

			virtual void setViewport(int width, int height, int offset_x = 0, int offset_y = 0);
			void loadCam(PerspectiveCamera3D& cam);

			const GeometryBuffer& getGBuffer() const;

			GBufferRenderer();
			virtual ~GBufferRenderer();


		};


	}

}