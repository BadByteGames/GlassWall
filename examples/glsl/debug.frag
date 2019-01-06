#version 130

in vec4 out_color;
in vec2 out_uv;
in vec3 out_normal;
in vec3 out_position;

uniform sampler2D textureSlot0;
uniform vec3 viewerpos;
uniform mat4 invertedmodel;

struct Light{
	float ambientstrength;
	float specularstrength;
	vec3 lightcolor;
	vec3 lightpos;
};

uniform Light lights[32];

out vec4 gl_FragColor;
void main(void){
	vec3 normal = normalize(mat3(invertedmodel) * out_normal);
	vec3 viewdir = normalize(viewerpos - out_position);
	/*vec3 ambient = lightcolor * vec3(ambientstrength);
	
	vec3 lightdir = normalize(lightpos - out_position);
	vec3 viewdir = normalize(viewerpos - out_position);
	vec3 reflectdir = reflect(-lightdir, normal);
	vec3 diffusion = lightcolor * max(dot(normal, lightdir), 0.0f);
	vec3 specular = lightcolor * specularstrength * pow(max(dot(viewdir, reflectdir), 0.0), 32);*/

	//vec4 lighting = vec4(ambient + diffusion + specular, 1.0f);

	vec4 lighting = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	for(int i = 0; i < lights.length(); i++){
		vec3 ambient = lights[i].lightcolor * lights[i].ambientstrength;
		vec3 lightdir = normalize(lights[i].lightpos - out_position);
		vec3 reflectdir = reflect(-lightdir, normal);
		vec3 diffusion = lights[i].lightcolor * max(dot(normal, lightdir), 0.0f);
		vec3 specular = lights[i].lightcolor * lights[i].specularstrength * pow(max(dot(viewdir, reflectdir), 0.0), 32);

		lighting.xyz += vec3(ambient  + diffusion + specular);
	}

	gl_FragColor= vec4(out_color)*texture2D(textureSlot0, out_uv) * lighting;
}