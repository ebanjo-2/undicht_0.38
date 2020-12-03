#ifndef GL33_TOOLS_H_INCLUDED
#define GL33_TOOLS_H_INCLUDED


#include "event_logger.h"

#define UND_DEBUG

#ifdef UND_DEBUG
#define UND_CHECK_GL_ERROR() undCheckGLError(UND_CODE_ORIGIN)
#else
#define UND_CHECK_GL_ERROR() // trying to save some time
#endif // UND_DEBUG



namespace undicht {

    namespace graphics {

        namespace gl33 {

            /** calls glGetError and translates the error code into text form
            * if not 0, the error get stored as a Note (UND_ERROR) */
            void undCheckGLError(const std::string& origin);

        } //gl33

    } // graphics

} // undicht



#endif // GL33_TOOLS_H_INCLUDED
