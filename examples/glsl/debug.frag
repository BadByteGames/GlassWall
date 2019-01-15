#version 130

in vec4 out_color;
in vec2 out_uv;
in vec3 out_normal;
in vec3 out_position;

uniform sampler2D textureSlot0;
uniform sampler2D textureSlot1;
uniform vec3 viewerpos;
uniform mat4 invertedmodel;

struct PointLight{
	float ambientstrength;
	float specularstrength;
	vec3 color;
	vec3 pos;

	float constant;
	float linear;
	float quadratic;
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

	vec3 lighting = vec3(0.0f);

	vec3 diffuseTex = vec3(texture2D(textureSlot0, out_uv));
	vec3 specularTex = vec3(texture2D(textureSlot1, out_uv));
	//caluclate directional light (sun)
	{
		vec3 ambient = directionallight.color * directionallight.ambientstrength;
		vec3 lightdir = normalize(-directionallight.direction);
		vec3 reflectdir = reflect(-lightdir, normal);
		vec3 diffusion = directionallight.color * max(dot(normal, lightdir), 0.0f);
		vec3 specular = directionallight.color * directionallight.specularstrength * pow(max(dot(viewdir, reflectdir), 0.0), 32);

		lighting.xyz += vec3(ambient * diffuseTex  + diffusion  * diffuseTex + specular * specularTex);
	}
	
	for(int i = 0; i < pointlights.length(); i++){
		if(pointlights[i].constant != 0){
			vec3 ambient = pointlights[i].color * pointlights[i].ambientstrength;
			vec3 lightdir = normalize(pointlights[i].pos - out_position);
			vec3 reflectdir = reflect(-lightdir, normal);
			vec3 diffusion = pointlights[i].color * max(dot(normal, lightdir), 0.0f);
			vec3 specular = pointlights[i].color * pointlights[i].specularstrength * pow(max(dot(viewdir, reflectdir), 0.0), 64);
			
			float distance = length(pointlights[i].pos - out_position);
			vec3 attenuation = vec3(pow(pointlights[i].constant + pointlights[i].linear * distance + pointlights[i].quadratic * (distance * distance), -1));    
			
			ambient *= diffuseTex * attenuation;
			diffusion *= diffuseTex * attenuation;
			specular *= specularTex * attenuation;

			lighting.xyz += ambient + diffusion + specular;
		}
	}

	gl_FragColor= vec4(out_color)*vec4(1.0, 1.0, 1.0, texture2D(textureSlot0, out_uv).a) * vec4(lighting, 1.0);
}