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
out vec3 normal;

vec2 calcUV(vec3 pos, vec3 normal);

void main() {

    vec3 pos = aPos * aBlockSize;
	
	uv = calcUV(pos, aNormal);
	material = aMaterial / 65536;
	normal = aNormal * 0.5 + 0.5;

	gl_Position = proj * view * vec4(pos + aBlockPos + chunk_offset, 1.0f);

}

vec2 calcUV(vec3 pos, vec3 normal) {

	vec3 uv_dir = normal * normal; // only positive values
	return uv_dir.x * pos.zy + uv_dir.y * pos.xz + uv_dir.z * pos.xy;
}

#fragment
#version 330 core

out vec2 uv_texture;
out float mat_texture;
out vec4 norm_texture; // 2 bits per channel are enough

in vec2 uv;
flat in float material;
in vec3 normal;

uniform sampler2DArray color;

void main() {

	uv_texture = uv - ivec2(uv);
	mat_texture = material;
	norm_texture = vec4(normal, 1.0f); // alpha is unused but needed to get to 1 byte per pixel
	
	//frag_color = texture(color, vec3(uv, material)).rgb;
	//frag_color = vec4(local_uv, 0, material / 65536); // material / 65536 if alpha is 2 bytes, it should be able to store the material
}
