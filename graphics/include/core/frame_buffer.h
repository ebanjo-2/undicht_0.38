#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <core/shared_lib_class.h>
#include <graphics/texture.h>


namespace undicht {

    namespace graphics {

        SHARED_LIB_DECL_BASE_CLASS(FrameBuffer, FrameBufferBase, createFrameBuffer, copyFrameBuffer, deleteFrameBuffer);


        SHARED_LIB_CLASS(class FrameBuffer: public FrameBufferBase {

        public:

            /** setting the size of the framebuffer and all of its attachments */
            virtual void setSize(int width, int height);

            virtual void getSize(int& width, int& height);

            /** @param texture: the framebuffer can only be used while the texture object exists
            * @param texture: it should have a pixel layout set before attachment, if not, default 24 bit rgb is going to be used for color attachments
            * @param attachment_type: UND_COLOR_ATTACHMENT, UND_DEPTH_ATTACHMENT_WRITE_ONLY or UND_DEPTH_ATTACHMENT_READ_AND_WRITE */
            virtual void addAttachment(graphics::Texture& texture, int attachment_type);

            /** @brief removes the attachment from the framebuffer */
            virtual void removeAttachment(graphics::Texture& texture);

            virtual void clearAttachments();

        public:

            FrameBuffer();
            virtual ~FrameBuffer();

        };)


    } // graphics

} // undicht


#endif // FRAME_BUFFER_H
