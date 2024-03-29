#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

// integers to distinguish data types
// i.e. to store the type of data of a buffer

namespace undicht {

    namespace tools {

		/** @return true, if the system stores the high bytes of data types
		* bigger than one byte,i.e. 32 bit int first (Big Endian) */
		bool isBigEndianSys();

    /** @return the size of the type in bytes */
    int getSizeOfType(int type);

    int getNumberOfComponents(int type);

    const extern int UND_TYPE_UNAVAILABLE;

    // primitive data types
    const extern int UND_INT;
	const extern int UND_UINT2; // 2 bits
	const extern int UND_UINT8; // 8 bit unsigned int, = UND_UNSIGNED_CHAR
	const extern int UND_UINT16; // = unsigned short
	const extern int UND_UINT32; // = default unsigned int

    const extern int UND_FLOAT;
    const extern int UND_STR;
    const extern int UND_UNSIGNED_CHAR;

    // boolean expressions
    const extern int UND_TRUE;
    const extern int UND_FALSE;

    const extern int UND_LESS;
    const extern int UND_GREATER;
    const extern int UND_EQUAL;
    const extern int UND_LEQUAL; // less or equal
    const extern int UND_GEQUAL; // greater or equal

    // mathematical data types
    const extern int UND_VEC4F; // 4 float vector
    const extern int UND_VEC3F; // 3 float vector
    const extern int UND_VEC3I; // 3 int vector
    const extern int UND_VEC3UI8; // 3 * unsigned char vector
    const extern int UND_VEC2F; // 2 float vector
	const extern int UND_VEC2I; // 2 int vector
    const extern int UND_MAT4F; // 4 * 4 float matrix
    const extern int UND_MAT3F; // 3 * 3 float matrix

    // input states
    const extern int UND_KEY_RELEASED; // 0
    const extern int UND_KEY_PRESSED;
    const extern int UND_KEY_REPEATED; // pressed for a longer time

    // texture parameters
    const extern int UND_NEAREST; // filtering
    const extern int UND_LINEAR;

    const extern int UND_REPEAT; // wrapping
    const extern int UND_CLAMP_TO_EDGE;

	// blending occurrs when the output of the fragment shader (dst color) gets mixed with another color
	// for example with what was already stored in the buffer that is being rendered to (src color)
	// blending factors translate directly into opengl blend factors
	// see https://learnopengl.com/Advanced-OpenGL/Blending
	const extern int UND_SRC_ALPHA;
	const extern int UND_ONE_MINUS_SRC_ALPHA;
	const extern int UND_DST_ALPHA;
	const extern int UND_ONE_MINUS_DST_ALPHA;

	// face culling
	const extern int UND_BACK_FACE;
	const extern int UND_FRONT_FACE;

    // attachments for Framebuffers
    /*const extern int UND_COLOR_RG_ATTACHMENT;
    const extern int UND_COLOR_RGB_ATTACHMENT;
    const extern int UND_COLOR_RGBA_ATTACHMENT;*/
    const extern int UND_COLOR_ATTACHMENT;
    const extern int UND_DEPTH_ATTACHMENT_WRITE_ONLY;
    const extern int UND_DEPTH_ATTACHMENT_READ_AND_WRITE;

    // update rates of buffers
    const extern int UND_RARE_UPDATE;// almost never
    const extern int UND_FREQUENT_UPDATE;// every other frame
    const extern int UND_CONSTANT_UPDATE;// each frame

    // shader types
    const extern int UND_VERTEX_SHADER;
    const extern int UND_GEOMETRY_SHADER;
    const extern int UND_FRAGMENT_SHADER;

    // buffer usage types
    const extern int UND_2D_TEXTURE_BUFFER; // the buffer stores 2D texture data
    const extern int UND_3D_TEXTURE_BUFFER; // the buffer stores multiple 2D textures (i.e. cubemap data)
    const extern int UND_SHADER_DATA_BUFFER; // the buffer is used to load one dimensional data to a shader
    const extern int UND_VERTEX_BUFFER; // storing vertices in the buffer
    const extern int UND_INDEX_BUFFER; // storing indices in the buffer

    // audio data formats
    const extern int UND_AUDIO_MONO8;
    const extern int UND_AUDIO_MONO16;
    const extern int UND_AUDIO_STEREO8;
    const extern int UND_AUDIO_STEREO16;

	// vertex attribute types
	/*const extern int UND_POS2F; // 2D position
	const extern int UND_POS3F; // 3D position
	const extern int UND_UV2F; // texture coordinate
	const extern int UND_NORMAL3F; // the direction the surface is pointing at
	const extern int UND_TANGENT3F; // used for normal mapping
	const extern int UND_BITANGENT3F; // used for normal mapping*/


    } // tools

} // undicht



#endif // TYPES_H_INCLUDED
