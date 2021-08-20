#vertex
#version 330 core


layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 proj;

uniform vec3 pos;

void main () {

	gl_Position = proj * view * vec4(pos + aPos, 1.0f);

}


#fragment 
#version 330 core


out vec3 out_color;

void main() {

	out_color = vec3(1,0,0);

}
