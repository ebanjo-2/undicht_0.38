#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <core/graphics_core.h>
#include <core/texture.h>


namespace undicht {

    namespace graphics {

        namespace interf {

            class FrameBuffer {

                public:

                    /** setting the size of the framebuffer and all of its attachments */
                    virtual void setSize(int width, int height) = 0;

                    virtual void getSize(int& width, int& height) = 0;

                    /** @param texture: the framebuffer can only be used while the texture object exists
                    * @param texture: it should have a pixel layout set before attachment, if not, default 24 bit rgb is going to be used for color attachments
                    * @param attachment_type: UND_COLOR_ATTACHMENT, UND_DEPTH_ATTACHMENT_WRITE_ONLY or UND_DEPTH_ATTACHMENT_READ_AND_WRITE */
                    virtual void addAttachment(graphics::Texture& texture, int attachment_type) = 0;

                    /** @brief removes the attachment from the framebuffer */
                    virtual void removeAttachment(graphics::Texture& texture) = 0;

                    virtual void clearAttachments() = 0;

                public:

                    FrameBuffer() = default;
                    virtual ~FrameBuffer() = default;

            };

        } // interf


    } // graphics

} // undicht

/////////////////////////////////////////////// including the classes implementing the Framebuffer class ////////////////////////////////////////////////////////

#ifdef USE_GL33

#include "core/gl33/gl33_frame_buffer.h"

namespace undicht {
	namespace graphics {
		typedef gl33::FrameBuffer FrameBuffer;
	} // graphics
} // undicht

#endif // USE_GL33

#endif // FRAME_BUFFER_H
