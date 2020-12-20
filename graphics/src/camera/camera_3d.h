#ifndef CAMERA_3D_H
#define CAMERA_3D_H

#include <math/orientation_3d.h>

namespace undicht {

	namespace graphics {

		class Camera3D : public tools::Orientation3D {

		public:

			glm::mat4 m_view_mat;
			glm::vec3 m_view_dir;

		protected:
			// function called when the transformation matrix needs to be updated, and with it the view matrix

			virtual void updateTransf();
			virtual void updateRotation();

		public:
			// functions returning camera matrices

			const glm::mat4& getViewMatrix();
			virtual const glm::mat4& getCameraProjectionMatrix() = 0;

			/** a unit vector pointing in the direction the camera is "facing" */
			const glm::vec3& getViewDirection();


			Camera3D();
			virtual ~Camera3D();

		};

	} // graphics

} // undicht

#endif // CAMERA_3D_H
