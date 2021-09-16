#vertex
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 0) in vec2 aUv;

out vec2 uv;

void main() {

	uv = aUv;

	gl_Position = vec4(aPos,0.0f,1.0f);

}

#fragment
#version 330 core

out vec3 FragColor;

in vec2 uv;

uniform sampler2D uv_texture;
uniform sampler2D normal_texture;

uniform sampler2DArray texture_atlas;

void main() {

	
	vec3 color_uv = texture(uv_texture, uv).rgb * vec3(1,1,255);
	
	FragColor = texture(texture_atlas, color_uv).rgb;
	//FragColor = vec4(1,0.1f,0.01, 1.0f);
}
