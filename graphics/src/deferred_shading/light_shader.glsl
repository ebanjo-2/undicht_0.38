#vertex 
#version 330 core

layout (location = 0) in vec3 aPos;

// camera uniforms
uniform mat4 proj;
uniform mat4 view;

// light uniforms
uniform vec3 light_origin = vec3(0,0,0);
uniform float light_radius = 50;


void main() {
	
		
	gl_Position = proj * view * vec4(aPos * light_radius + light_origin, 1.0f);

}

#fragment
#version 330 core

out vec4 light_color_intensity;

// camera uniforms
uniform float viewrange;
uniform vec3 cam_pos;

uniform vec2 viewport_size = vec2(1.0f / 800.0f, 1.0f / 600.0f);

// light uniforms
uniform vec3 light_color = vec3(1,1,0.5);
uniform vec3 light_origin = vec3(0,0,0);
uniform float light_radius = 50;
uniform float db_light_radius = 1 / 50;

// material values that influenz lighting
uniform float shininess = 8;
uniform float specularStrength = 0.7;

// input textures from the geometry buffer
uniform sampler2D position;
uniform sampler2D normal;

// functions to read from the geometry buffer textures
vec3 getFragPosition(vec2 _uv);
vec3 getFragNormal(vec2 _uv);

// functions to calculate phong lighting
float diffuse(vec3 normal, vec3 direction_to_light);
float specular(vec3 normal, vec3 direction_to_light, vec3 direction_to_cam);



void main(){
	
	vec2 uv = gl_FragCoord.xy * viewport_size;

	
	// getting the data from the textures
	vec3 obj_position = getFragPosition(uv);
	vec3 obj_normal = getFragNormal(uv); 
	
	// vectors used for phong lighting
	vec3 dir_to_cam = normalize(cam_pos - obj_position);
	vec3 dir_to_light = normalize(light_origin - obj_position);
	
	// calculating how much the light effects the object (without taking its distance into account)
	float phong = diffuse(obj_normal, dir_to_light) + specular(obj_normal, dir_to_light, dir_to_cam);
	
	// calculating the lights intensity at the position of fragment
	float distance_to_light = length(obj_position - light_origin);
	float light_strength = max((light_radius - distance_to_light) * db_light_radius, 0);
	
	light_color_intensity = vec4(light_color * phong, light_strength);

}

vec3 getFragPosition(vec2 _uv) {
	// @return the position of the fragment extracted from the position texture 
	
	vec3 position_rel_cam = ((texture(position, _uv).xyz) * 2 - 1) * viewrange;
	
	/*if(length(position_rel_cam) > viewrange){
		discard;
	}*/
	
	return position_rel_cam + cam_pos;
}

vec3 getFragNormal(vec2 _uv) {
	
	return texture(normal, _uv).xyz * 2 - 1;
}


float diffuse(vec3 normal, vec3 direction_to_light){
	
	return max(dot(direction_to_light, normal),0.0);
}

float specular(vec3 normal, vec3 direction_to_light, vec3 direction_to_cam){

	vec3 reflectDir = reflect(- direction_to_light, normal);
	float spec = pow(max(dot(direction_to_cam, reflectDir), 0.0), shininess);
	
	return specularStrength * spec;
}