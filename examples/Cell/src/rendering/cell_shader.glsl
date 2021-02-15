#vertex
#version 330 core

// per vertex data
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

// per instance data
layout (location = 3) in vec3 aBlockPos;
layout (location = 4) in vec3 aBlockSize;
layout (location = 5) in float aMaterial; // opengl doesnt seem to like integers


// camera uniforms
uniform mat4 view;
uniform mat4 proj;

out vec2 uv;
out float material;


void main() {

	uv = aUV;
	material = aMaterial;

	gl_Position = proj * view * vec4((aPos * aBlockSize) + aBlockPos, 1.0f);
	//gl_Position = proj * view * vec4(aPos * 10, 1.0f);
}

#fragment
#version 330 core

out vec3 frag_color;

in vec2 uv;
in float material;

uniform sampler2DArray color;

void main() {

	frag_color = texture(color, vec3(uv, material)).rgb;

}