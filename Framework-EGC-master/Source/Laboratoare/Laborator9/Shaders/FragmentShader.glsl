#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
 
in vec2 texcoord;


// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

layout(location = 0) out vec4 out_color;

void main()
{

vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);

	// TODO: define ambient light component
	float ambient_light = material_kd * 0.25;

	// TODO: compute diffuse light component
	float diffuse_light = material_kd * pow(dot(L, normalize(world_normal)), 0);

	// TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)	{
		specular_light = material_ks * pow(max(dot(normalize(world_normal), H), 0), material_shininess);
	}

	// TODO: compute light
	float d = distance(light_position, world_position);
	float attenuation = 1 / pow(d, 2);

	//spot-light
	float cut_off = radians(30.f);
	float spot_light = dot(-L, light_direction);
	float spot_light_limit = cos(cut_off);

	// Quadratic attenuation
	float linear_att = 0;
	float light_att_factor = 0;

	if (spot_light > cos(cut_off)) {
		linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
		light_att_factor = pow(linear_att, 2);
	}
	float light =  ambient_light + attenuation * (diffuse_light + specular_light);



	// TODO : calculate the out_color using the texture2D() function
	vec4 color1 = texture2D(texture_1, texcoord);
	if (color1.a < 0.5f) { discard; }

	vec4 color2 = texture2D(texture_2, texcoord);
	if (color2.a < 0.5f) { discard; }

	vec4 color = mix(color1, color2, 0.5f);
	out_color = color + light;
}