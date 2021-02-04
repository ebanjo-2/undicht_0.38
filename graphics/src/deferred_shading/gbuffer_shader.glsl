#vertex 
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNormal;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

uniform vec3 cam_pos;
uniform float dvbvr;

out vec2 uv;
out vec3 normal;
out vec3 pos_to_cam;

void main() {
	
	uv = aUv;
	normal = (aNormal + vec3(1.0)) * 0.5;
	
	vec4 worldpos = /*model * */vec4(aPos, 1.0f);
	
	pos_to_cam = (worldpos.xyz - cam_pos) * dvbvr + 0.5; // making all the information fit in a texture;

	gl_Position = proj * view * worldpos;

}

#fragment
#version 330 core

layout (location = 0) out vec3 albedo_out;
layout (location = 1) out vec3 position_out;
layout (location = 2) out vec3 normal_out;


in vec2 uv;
in vec3 normal;
in vec3 pos_to_cam;

uniform sampler2D color;

void main(){
	
	position_out = pos_to_cam;
	normal_out = normal;
	albedo_out = texture(color, uv).rgb;

}

