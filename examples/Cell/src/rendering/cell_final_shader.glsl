#vertex
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUv;

out vec2 uv;

void main() {

	uv = aUv;

	gl_Position = vec4(aPos,0.0f,1.0f);

}




#fragment
#version 330 core

out vec3 FragColor;

in vec2 uv;

uniform usampler2D geo_texture;
uniform sampler2D depth_map;
uniform sampler2D light_map;

uniform sampler2DArray texture_atlas;

uniform mat4 inv_view;
uniform mat4 inv_proj;

uniform mat4 view;
uniform mat4 proj;


const int TEX_ATLAS_WIDTH = 512;
const int TEX_ATLAS_HEIGHT = 512;

const vec2 uv_size = vec2(32.0f / TEX_ATLAS_WIDTH, 32.0f / TEX_ATLAS_HEIGHT);

vec3 getWorldPosition();
vec2 getTexUV(vec3 pos, int face, uint material);


void main() {

    uint material = texture(geo_texture, uv).x;
    int face = int(texture(geo_texture, uv).y);
       
    vec2 tex_uv = getTexUV(getWorldPosition(), face, material);
	
    //FragColor = getWorldPosition().xyz / 16;

    vec3 lighting = texture(light_map, uv).rgb + 0.15;
    
     FragColor = texture(texture_atlas, vec3(tex_uv, material / uint(256))).rgb * lighting;
     //FragColor = lighting;
}


vec3 getWorldPosition() {
    // thanks for the math 
    // https://stackoverflow.com/questions/32227283/getting-world-position-from-depth-buffer-value

    float z = texture(depth_map, uv).x * 2.0 - 1.0;

    vec4 clipSpacePosition = vec4(uv * 2.0 - 1.0, z, 1.0);
    vec4 viewSpacePosition = inv_proj * clipSpacePosition;

    // Perspective division
    viewSpacePosition /= viewSpacePosition.w;

    vec4 worldSpacePosition = inv_view * viewSpacePosition;

    return worldSpacePosition.xyz;
    
    //return viewSpacePosition.xyz;
}

vec2 getTexUV(vec3 pos, int face, uint material) {

    vec3 rep_pos = fract(pos); // repeats from 0 to 1

	vec2 rep_uv = 
		float(bool(face & 0x03)) * fract(rep_pos).zx + 
		float(bool(face & 0x0C)) * fract(rep_pos).zy + 
		float(bool(face & 0x30)) * fract(rep_pos).xy;

    vec2 tex_uv = vec2(material % uint(16), (material % uint(256)) / uint(16)) * uv_size;
    tex_uv += uv_size * rep_uv;    
       
    return tex_uv;
}
