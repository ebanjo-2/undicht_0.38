#ifdef FRAME_BUFFER_H // this file should only be included via include/core/frame_buffer.h

#ifndef GL33_FRAME_BUFFER_H
#define GL33_FRAME_BUFFER_H

#include <shared_id.h>

namespace undicht {

	namespace graphics {

		namespace gl33 {

			class FrameBuffer : public interf::FrameBuffer {

			public:
				// opengl only members / functions

				int m_width = 0;
				int m_height = 0;

				undicht::tools::SharedID m_id;
				int m_type; // will always be GL_FRAMEBUFFER

				std::vector<int> m_attachment_types;
				std::vector<Texture*> m_attachments;

				std::vector<int> m_color_outputs;

				virtual int getColorOutputCount();

				virtual void updateColorOutputs();

				virtual void checkStatus();

				virtual void bind();

				static void bind(int id);

			public:

				/** setting the size of the framebuffer and all of its attachments */
				virtual void setSize(int width, int height);

				virtual void getSize(int& width, int& height);

				/** @param texture: the framebuffer can only be used while the texture object exists
				* @param texture: it should have a pixel layout set before attachment, if not, default 24 bit rgb is going to be used for color attachments
				* @param attachment_type: UND_COLOR_ATTACHMENT, UND_DEPTH_ATTACHMENT_WRITE_ONLY or UND_DEPTH_ATTACHMENT_READ_AND_WRITE 
				* @param location: the location under which the output buffer can be accessed in a shader */
				virtual void addAttachment(graphics::Texture& texture, int attachment_type, int location = 0);

				/** @brief removes the attachment from the framebuffer */
				virtual void removeAttachment(graphics::Texture& texture);

				virtual void clearAttachments();


			public:


				FrameBuffer();
				virtual ~FrameBuffer();

			};

		} // gl33

	} // graphics

} // undicht


#endif // GL33_FRAME_BUFFER_H

#endif // FRAME_BUFFER_H