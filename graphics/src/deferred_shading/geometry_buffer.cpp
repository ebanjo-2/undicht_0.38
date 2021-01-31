#include "geometry_buffer.h"
#include <types.h>

namespace undicht {

	namespace graphics {

		using namespace tools;

#define DEPTH_BUFFER 0
#define COLOR_BUFFER 1
#define POSITION_BUFFER 2
#define NORMAL_BUFFER 3

		GeometryBuffer::GeometryBuffer() {


		}


		GeometryBuffer::~GeometryBuffer() {


		}

		///////////////////////////////////// managing the buffers used to store the geometry data /////////////////////////////////////

		void GeometryBuffer::setDepthBuffer(bool use_depth) {

			if (use_depth && !m_depth_buffer) {
				// enabling the depth buffer

				m_buffers[DEPTH_BUFFER].setName("depth");
				addAttachment(m_buffers[DEPTH_BUFFER], UND_DEPTH_ATTACHMENT_READ_AND_WRITE);
				m_depth_buffer = true;

			} else if ((!use_depth) && !m_depth_buffer) {
				// disabling the depth buffer

				removeAttachment(m_buffers.at(DEPTH_BUFFER)); // not jet implemented by gl33::Framebuffer
				m_depth_buffer = false;
			}
		}


		void GeometryBuffer::setColorBuffer(bool use_color) {

			if (use_color && !m_color_buffer) {
				// enabling the color buffer

				m_buffers[COLOR_BUFFER].setName("color");
				addAttachment(m_buffers[COLOR_BUFFER], UND_COLOR_ATTACHMENT, 0);
				m_color_buffer = true;

			}
			else if ((!use_color) && !m_color_buffer) {
				// disabling the color buffer

				removeAttachment(m_buffers.at(COLOR_BUFFER)); // not jet implemented by gl33::Framebuffer
				m_color_buffer = false;
			}

		}


		void GeometryBuffer::setPositionBuffer(bool use_position ) {

			if (use_position && !m_position_buffer) {
				// enabling the position buffer

				m_buffers[POSITION_BUFFER].setName("position");
				addAttachment(m_buffers[POSITION_BUFFER], UND_COLOR_ATTACHMENT, 1);
				m_position_buffer = true;

			}
			else if ((!use_position) && !m_position_buffer) {
				// disabling the position buffer

				removeAttachment(m_buffers.at(POSITION_BUFFER)); // not jet implemented by gl33::Framebuffer
				m_position_buffer = false;
			}
		}


		void GeometryBuffer::setNormalBuffer(bool use_normal) {

			if (use_normal && !m_normal_buffer) {
				// enabling the position buffer

				m_buffers[NORMAL_BUFFER].setName("normal");
				addAttachment(m_buffers[NORMAL_BUFFER], UND_COLOR_ATTACHMENT, 2);
				m_normal_buffer = true;

			}
			else if ((!use_normal) && !m_normal_buffer) {
				// disabling the position buffer

				removeAttachment(m_buffers.at(NORMAL_BUFFER)); // not jet implemented by gl33::Framebuffer
				m_normal_buffer = false;
			}

		}


		const Texture* GeometryBuffer::getDepthBuffer() {

			if (m_depth_buffer) {

				return &m_buffers.at(m_depth_buffer);
			} else {

				return 0;
			}
		}


		const Texture* GeometryBuffer::getColorBuffer() {

			if (m_color_buffer) {

				return &m_buffers.at(COLOR_BUFFER);
			}
			else {

				return 0;
			}
		}


		const Texture* GeometryBuffer::getPositionBuffer() {

			if (m_position_buffer) {

				return &m_buffers.at(POSITION_BUFFER);
			} else {

				return 0;
			}
		}


		const Texture* GeometryBuffer::getNormalBuffer() {

			if (m_normal_buffer) {

				return &m_buffers.at(NORMAL_BUFFER);
			}
			else {

				return 0;
			}
		}


	} // graphics

} // undicht