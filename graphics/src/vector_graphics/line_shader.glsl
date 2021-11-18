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
uniform vec2 pos1;
uniform float width;

uniform vec4 color = vec4(1);


in vec2 screen_pos;

void main() {

    // calculating the distance between the line between pos0 and pos1
    float line_length = length(pos1 - pos0);
    vec2 v = (pos1 - pos0) / line_length;
    vec2 u = screen_pos - pos0;
    float vf = dot(u, v); // factor with which v gets multiplied to get to the nearest position to screen_pos
    vec2 c = u - (vf * v); // "connection" between the line and screen_pos

    float d = length(c); // distance between the line and screen_pos
    
    if(vf > line_length)
        d = max(d, vf - line_length);
    
    if(vf < 0)
        d = max(d, -1.0f * vf);
    

    // using 1 / (d * 100.0f) as a value for a color channel gives cool results
    //out_color = vec4(0,1.0f / (d * 100.0f), 0,1);


	 	// (width - d) will be negative, if screen_pos is outside of the line
   	float alpha = min(1.0f, (width - d) * 200.0f);

   out_color = vec4(color.rgb, color.a * alpha);

}


