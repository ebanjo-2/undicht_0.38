#vertex
#version 330 core

layout (location = 0) in vec2 aPos;

out vec2 uv;

void main() {

	uv = aPos / 2 + 0.5;

	gl_Position = vec4(aPos, 1.0f, 1.0f);
}

#fragment
#version 330 core

out vec3 out_color;

in vec2 uv;

uniform sampler2D noise;

void main () {

	out_color = texture(noise, uv).rgb;

}

