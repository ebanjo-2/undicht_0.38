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
out vec2 uv0;
out vec2 uv1;

out float face_shade;

uniform mat4 view;
uniform mat4 proj;

uniform ivec3 chunk_pos;


const int TEX_ATLAS_WIDTH = 256;
const int TEX_ATLAS_HEIGHT = 256;

const vec2 uvsize = vec2(16.0f / TEX_ATLAS_WIDTH, 16.0f / TEX_ATLAS_HEIGHT);

void main () {

		int mat = int(matf); // convert to int

    uv0 = vec2(mat % 16, mat / 16) * uvsize;
    uv1 = uv0 + uvsize;
    
    int aFacei = int(aFace);
    
    vec3 cell_size = pos1 - pos0;
    
    face_shade = float(bool(aFacei & 0x01)) * 1.0f + float(bool(aFacei & 0x02)) * 0.2f  + float(bool(aFacei & 0x0C)) * 0.6f + float(bool(aFacei & 0x30)) * 0.4f;


    //uv = vec3((vec2(1.0f, 1.0f) - aUv) * uv0 + aUv * uv1, mat / 256);
		uv.xy = aUv * (float(bool(aFacei & 0x03)) * cell_size.zx + float(bool(aFacei & 0x0C)) * cell_size.zy + float(bool(aFacei & 0x30)) * cell_size.xy);
		uv.z = mat / 256;

		vec3 vertex_pos = pos0 * (vec3(1,1,1) - aPos) + pos1 * aPos;
    bool draw_face = bool(aFacei & int(facef)); // 1 if the face should get drawn, 0 if not
    
	  gl_Position = proj * view * vec4(chunk_pos + vertex_pos * float(draw_face), 1.0f);
	//gl_Position = proj * view * vec4(aPos, 1.0f);
}


#fragment 
#version 330 core

out vec3 uv_texture;
out vec4 normal_texture;

in vec3 uv;
in vec2 uv0;
in vec2 uv1;

in float face_shade;




void main() {

	vec2 repeating_uv = uv.xy - ivec2(uv.xy);


	uv_texture.xy = (vec2(1.0f, 1.0f) - repeating_uv) * uv0 + repeating_uv * uv1;
  uv_texture.z = uv.z;
  
  normal_texture = vec4(1,0,0,1);

}
