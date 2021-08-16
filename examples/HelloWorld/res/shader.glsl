#vertex 
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNormal;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec2 uv;

void main() {
	
	uv = aUv;
	
	gl_Position = proj * view * vec4(aPos.xzy, 1.0f);

}

#fragment
#version 330 core

out vec4 frag_color;

in vec2 uv;

uniform sampler2D color;

void main(){
	
	frag_color = texture(color, uv);

}