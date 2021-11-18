#vertex
#version 330 core

layout (location = 0) in vec2 aPos; // will always go from 0 to 1

out vec2 screen_pos;

uniform float aspect_ratio = 1;

void main() {

    screen_pos = aPos * vec2(aspect_ratio, 1);

    gl_Position = vec4(aPos, 1.0f, 1.0f);

}

#fragment
#version 330 core

out vec4 out_color;

uniform vec2 pos0;
uniform float radius;
uniform float width;

uniform vec4 color = vec4(1);

in vec2 screen_pos;

void main() {
	
		float d = abs(length(pos0 - screen_pos) - radius); // the distance from the circle line
		
		// (width - d) will be negative, if screen_pos is outside of the line
    float alpha = min(1.0f, (width - d) * 200.0f);

    out_color = vec4(color.rgb, color.a * alpha);

}
