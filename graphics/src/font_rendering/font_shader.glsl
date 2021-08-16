#vertex
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUv;

// per instance data
layout (location = 2) in vec2 aPos0;
layout (location = 3) in vec2 aPos1;
layout (location = 4) in float aAdv;
layout (location = 5) in vec2 aOffs;

out vec2 uv;

uniform vec2 font_map_size;
uniform vec2 font_scale;
uniform vec2 text_pos;

void main() {

	uv = aUv;
	
	uv = aPos0 * (vec2(1,1) - aUv) + aPos1 * aUv;

	vec2 char_size = abs(aPos1 - aPos0);
	vec2 screen_pos = text_pos + vec2(1,0) * aAdv * font_scale + vec2(1, -1) * aOffs * font_scale; // upper left corner
	
	//vec2 screen_pos = text_pos + aPos * 0.1 + vec2(1,0) * aAdv * font_scale;
	
	//vec2 screen_pos = text_pos + aPos * char_size * font_scale;
	//screen_pos.x += aAdv * font_scale;
	
	//gl_Position = vec4(aPos, 1.0, 1.0);
	gl_Position = vec4(screen_pos + char_size * aPos * font_scale , 1.0, 1.0);
	
}

#fragment
#version 330 core

out vec4 out_color;

in vec2 uv;

uniform sampler2D fontmap;

uniform vec3 font_color = vec3(0,0,0);

void main() {
	
	//out_color = vec3(uv, 0,0);
	out_color = vec4(font_color,texture(fontmap, uv).x);

}
