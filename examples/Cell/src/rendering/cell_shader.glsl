#vertex
#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

out vec3 uv;

uniform mat4 view;
uniform mat4 proj;

uniform vec3 chunk_pos;

uniform vec3 pos0;
uniform vec3 pos1;

uniform int mat = 3;

const int TEX_ATLAS_WIDTH = 256;
const int TEX_ATLAS_HEIGHT = 256;

const vec2 uvsize = vec2(16.0f / TEX_ATLAS_WIDTH, 16.0f / TEX_ATLAS_HEIGHT);

void main () {

    vec2 uv0 = vec2(mat % 16, mat / 16) * uvsize;
    vec2 uv1 = uv0 + uvsize;

    //uv = vec3((vec2(1.0f, 1.0f) - aUv) * uv0 + aUv * uv1, mat / 256);
    uv = vec3(aUv, 0);
    
	gl_Position = proj * view * vec4(chunk_pos + pos0 * (vec3(1,1,1) - aPos) + pos1 * aPos, 1.0f);

}


#fragment 
#version 330 core

in vec3 uv;

out vec3 out_color;

uniform sampler2DArray texture_atlas;

void main() {
    
    //out_color = vec3(1,0,0);
	out_color = texture(texture_atlas, uv).rgb;
    //out_color = vec3(uv, 0);
}
