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

// chunk uniforms
uniform ivec3 chunk_offset = ivec3(0,0,0);

out vec2 uv;
flat out float material;


void main() {

    vec3 pos = aPos * aBlockSize;
	
	vec3 uv_dir = aNormal * aNormal; // only positive values
	uv = uv_dir.x * pos.zy + uv_dir.y * pos.xz + uv_dir.z * pos.xy;
	
	material = aMaterial;

	gl_Position = proj * view * vec4(pos + aBlockPos + chunk_offset, 1.0f);

}

#fragment
#version 330 core

out vec2 uv_texture;
out uint mat_texture;
out vec2 norm_texture; // 2 bits per channel are enough

in vec2 uv;
flat in float material;

uniform sampler2DArray color;

void main() {

	uv_texture = uv - ivec2(uv);

	//frag_color = texture(color, vec3(uv, material)).rgb;
	//frag_color = vec4(local_uv, 0, material / 65536); // material / 65536 if alpha is 2 bytes, it should be able to store the material
}
