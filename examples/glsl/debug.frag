#version 130

in vec4 out_color;
in vec2 out_uv;
in vec3 out_normal;
in vec3 out_position;

uniform sampler2D textureSlot0;
uniform vec3 viewerpos;
uniform mat4 invertedmodel;

struct PointLight{
	float ambientstrength;
	float specularstrength;
	vec3 color;
	vec3 pos;
};

struct DirectionalLight{
	float ambientstrength;
	float specularstrength;
	vec3 color;
	vec3 direction;
};

uniform DirectionalLight directionallight;

uniform PointLight pointlights[32];

out vec4 gl_FragColor;
void main(void){
	vec3 normal = normalize(mat3(invertedmodel) * out_normal);
	vec3 viewdir = normalize(viewerpos - out_position);

	vec4 lighting = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	//caluclate directional light (sun)
	{
		vec3 ambient = directionallight.color * directionallight.ambientstrength;
		vec3 lightdir = normalize(-directionallight.direction);
		vec3 reflectdir = reflect(-lightdir, normal);
		vec3 diffusion = directionallight.color * max(dot(normal, lightdir), 0.0f);
		vec3 specular = directionallight.color * directionallight.specularstrength * pow(max(dot(viewdir, reflectdir), 0.0), 32);

		lighting.xyz += vec3(ambient  + diffusion + specular);
	}

	for(int i = 0; i < pointlights.length(); i++){
		vec3 ambient = pointlights[i].color * pointlights[i].ambientstrength;
		vec3 lightdir = normalize(pointlights[i].pos - out_position);
		vec3 reflectdir = reflect(-lightdir, normal);
		vec3 diffusion = pointlights[i].color * max(dot(normal, lightdir), 0.0f);
		vec3 specular = pointlights[i].color * pointlights[i].specularstrength * pow(max(dot(viewdir, reflectdir), 0.0), 32);

		lighting.xyz += vec3(ambient  + diffusion + specular);
	}

	gl_FragColor= vec4(out_color)*texture2D(textureSlot0, out_uv) * lighting;
}