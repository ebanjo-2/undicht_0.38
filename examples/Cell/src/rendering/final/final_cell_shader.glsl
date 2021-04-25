#vertex
#version 330 core

layout (location = 0) in vec2 aPos;

out vec2 uv;

void main() {

	uv = aPos * 0.5 + 0.5;

	gl_Position = vec4(aPos,0.0f,1.0f);

}

#fragment
#version 330 core

out vec3 FragColor;

in vec2 uv;

uniform sampler2D uvs;
uniform sampler2D material;
uniform sampler2D normal;

uniform sampler2DArray texture_atlas;

void main() {

	float material_id = texture(material, uv).r * 65536;
	vec3 tex_atlas_uv = vec3(texture(uvs, uv).rg, material_id);
	
	vec3 frag_normal = texture(normal, uv).rgb * 2 - 1;
	
	vec3 sun_dir = normalize(vec3(0,1,1));
	float basic_lighting = dot(frag_normal, sun_dir) + 0.3;


	FragColor = texture(texture_atlas, tex_atlas_uv).rgb * basic_lighting;
	//FragColor = vec4(1,0.1f,0.01, 1.0f);
}