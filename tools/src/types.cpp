#include "types.h"

// integers to distinguish data types
// i.e. to store the type of data of a buffer

namespace undicht {

    namespace tools {

        const int UND_TYPE_UNAVAILABLE = 0;

        // primitive data types
        const int UND_INT = 1;
        const int UND_FLOAT = 2;
        const int UND_STR = 3;
        const int UND_UNSIGNED_CHAR = 4;

        // boolean expressions
        const int UND_TRUE = 1;
        const int UND_FALSE = 0;

        const int UND_LESS = 2;
        const int UND_GREATER = 3;
        const int UND_EQUAL = 4;
        const int UND_LEQUAL = 5; // less or equal
        const int UND_GEQUAL = 6; // greater or equal

        // mathematical data types
        const int UND_VEC4F = 100;
        const int UND_VEC3F = 101; // 3 float vector
        const int UND_VEC3I = 102; // 3 int vector
        const int UND_VEC2F = 103; // 2 float vector
        const int UND_MAT4F = 104; // 4 * 4 float matrix
        const int UND_MAT3F = 105; // 3 * 3 float matrix

        // input states
        const int UND_KEY_RELEASED = 0; // 0
        const int UND_KEY_PRESSED = 1;
        const int UND_KEY_REPEATED = 2; // pressed for a longer time

        // texture parameters
        const int UND_NEAREST = 0; // filtering
        const int UND_LINEAR = 1;

        const int UND_REPEAT = 0; // wrapping
        const int UND_CLAMP_TO_EDGE = 1;

        // attachments for Framebuffers
        /*const int UND_COLOR_RG_ATTACHMENT = 0;
        const int UND_COLOR_RGB_ATTACHMENT = 1;
        const int UND_COLOR_RGBA_ATTACHMENT = 2;*/
        const int UND_COLOR_ATTACHMENT = 0;
        const int UND_DEPTH_ATTACHMENT_WRITE_ONLY = 3;
        const int UND_DEPTH_ATTACHMENT_READ_AND_WRITE = 4;

        // update rates of buffers
        const int UND_RARE_UPDATE = 0;// almost never
        const int UND_FREQUENT_UPDATE = 1;// every other frame
        const int UND_CONSTANT_UPDATE = 2;// each frame

        // shader types
        const int UND_VERTEX_SHADER = 0;
        const int UND_GEOMETRY_SHADER = 1;
        const int UND_FRAGMENT_SHADER = 2;

        // buffer usage types
        const int UND_2D_TEXTURE_BUFFER = 0; // the buffer stores 2D texture data
        const int UND_3D_TEXTURE_BUFFER = 1; // the buffer stores multiple 2D textures (i.e. cubemap data)
        const int UND_SHADER_DATA_BUFFER = 2; // the buffer is used to load one dimensional data to a shader
        const int UND_VERTEX_BUFFER = 3; // storing vertices in the buffer
        const int UND_INDEX_BUFFER = 4; // storing indices in the buffer

        // audio data formats
        const int UND_AUDIO_MONO8 = 1;
        const int UND_AUDIO_MONO16 = 2;
        const int UND_AUDIO_STEREO8 = 3;
        const int UND_AUDIO_STEREO16 = 4;

        int getSizeOfType(int type) {
            /** @return the size of the type in bytes */

            if(type == UND_INT)
                return sizeof(int);
            if(type == UND_FLOAT)
                return sizeof(float);
            if(type == UND_VEC4F)
                return 4 * sizeof(float);
            if(type == UND_VEC3F)
                return 3 * sizeof(float); // 3 float vector
            if(type == UND_VEC3I)
                return 3 * sizeof(int); // 3 int vector
            if(type == UND_VEC2F)
                return 2 * sizeof(float); // 2 float vector
            if(type == UND_MAT4F)
                return 16 * sizeof(float); // 4 * 4 float matrix
            if(type == UND_MAT3F)
                return 9 * sizeof(float); // 3 * 3 float matrix

            return 0;
        }

        int getNumberOfComponents(int type) {
            /** @return the number of single types used in the type
            * @example : UND_VEC3F consists of 3 floats, so @return would be 3 */

            if(type == UND_INT)
                return 1;
            if(type == UND_FLOAT)
                return 1;
            if(type == UND_VEC4F)
                return 4;
            if(type == UND_VEC3F)
                return 3; // 3 float vector
            if(type == UND_VEC3I)
                return 3; // 3 int vector
            if(type == UND_VEC2F)
                return 2; // 2 float vector
            if(type == UND_MAT4F)
                return 16; // 4 * 4 float matrix
            if(type == UND_MAT3F)
                return 9; // 3 * 3 float matrix

            return 0;
        }


    } // tools

} // undicht
