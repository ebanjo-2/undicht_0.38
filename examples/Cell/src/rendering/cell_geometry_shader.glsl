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

flat out float material; // expecting a float in the range of 0 to 1 (multiply by 256 * 256 to get the material id)
flat out float face; // expecting a float in the range of 0 to 1 (multiply by 256 to get the face id)

uniform mat4 view;
uniform mat4 proj;

uniform ivec3 chunk_pos;


const int TEX_ATLAS_WIDTH = 256;
const int TEX_ATLAS_HEIGHT = 256;

const vec2 uv_advance = vec2(16.0f / TEX_ATLAS_WIDTH, 16.0f / TEX_ATLAS_HEIGHT);
const vec2 uvsize = vec2(15.0f / TEX_ATLAS_WIDTH, 15.0f / TEX_ATLAS_HEIGHT);

// the normal as it gets stored in the normal texture
// multiply with 2 and subtract 1 to get the real normal
vec3 getTexNormal(int face);
vec2 getUvScale(int face, vec3 cell_size);

void main () {

	material = matf;
	face = aFace;

	vec3 vertex_pos = pos0 * (vec3(1,1,1) - aPos) + pos1 * aPos;
    bool draw_face = bool(int(aFace) & int(facef)); // 1 if the face should get drawn, 0 if not
    
	gl_Position = proj * view * vec4(chunk_pos + vertex_pos * float(draw_face), 1.0f);
	//gl_Position = proj * view * vec4(aPos, 1.0f);
}



#fragment 
#version 330 core


//out uvec2 uv_texture;
out uvec2 geo_texture;

flat in float material; // expecting a float in the range of 0 to 1 (multiply by 256 * 256 to get the material id)
flat in float face; // expecting a float in the range of 0 to 1 (multiply by 256 to get the face id)

void main() {


    geo_texture = uvec2(material, face);
    //geo_texture = uvec2(0, 1);
}
