#vertex
#version 330 core


// updated per vertex
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in float aFace;

// updated per instance
layout (location = 3) in vec3 pos0;
layout (location = 4) in vec3 pos1;
layout (location = 5) in float matf;
layout (location = 6) in float facef;


//const int mat = 1;

out vec3 uv;

uniform mat4 view;
uniform mat4 proj;

uniform vec3 chunk_pos;


const int TEX_ATLAS_WIDTH = 256;
const int TEX_ATLAS_HEIGHT = 256;

const vec2 uvsize = vec2(16.0f / TEX_ATLAS_WIDTH, 16.0f / TEX_ATLAS_HEIGHT);

void main () {

		int mat = int(matf); // convert to int

    vec2 uv0 = vec2(mat % 16, mat / 16) * uvsize;
    vec2 uv1 = uv0 + uvsize;

    uv = vec3((vec2(1.0f, 1.0f) - aUv) * uv0 + aUv * uv1, mat / 256);


		vec3 vertex_pos = pos0 * (vec3(1,1,1) - aPos) + pos1 * aPos;
    bool draw_face = bool(int(aFace) & int(facef)); // 1 if the face should get drawn, 0 if not
    
	  gl_Position = proj * view * vec4(chunk_pos + vertex_pos * float(draw_face), 1.0f);
	//gl_Position = proj * view * vec4(aPos, 1.0f);
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
