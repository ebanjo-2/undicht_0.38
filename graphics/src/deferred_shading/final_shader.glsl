#vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;


out vec2 uv;

void main() {

	uv = aUv;
	
	gl_Position = vec4(aPos, 1.0f);
	
}

#fragment
#version 330 core

out vec4 out_frag_color;
in vec2 uv;

uniform sampler2D color;
uniform sampler2D light_map;


void main() {

	out_frag_color = texture(color, uv) * (texture(light_map, uv) + 0.1);
		
	//out_frag_color = texture(light_map, uv);
	
}

