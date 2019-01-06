#version 130

in vec4 out_color;
in vec2 out_uv;
in vec3 out_normal;
in vec3 out_position;

uniform sampler2D textureSlot0;
uniform float ambientstrength;
uniform float specularstrength;
uniform vec3 lightpos;
uniform vec3 lightcolor;
uniform vec3 viewerpos;
uniform mat4 invertedmodel;

out vec4 gl_FragColor;
void main(void){
	vec3 ambient = lightcolor * vec3(ambientstrength);
	
	vec3 normal = normalize(mat3(invertedmodel) * out_normal);
	vec3 lightdir = normalize(lightpos - out_position);
	vec3 viewdir = normalize(viewerpos - out_position);
	vec3 reflectdir = reflect(-lightdir, normal);
	vec3 diffusion = lightcolor * max(dot(normal, lightdir), 0.0f);
	vec3 specular = lightcolor * specularstrength * pow(max(dot(viewdir, reflectdir), 0.0), 32);

	vec4 lighting = vec4(ambient + diffusion+specular, 1.0f);
	gl_FragColor= vec4(out_color)*texture2D(textureSlot0, out_uv) * lighting;
}