#version 130

in vec4 out_color;
in vec2 out_uv;
in vec3 out_normal;
in vec3 out_position;

uniform sampler2D textureSlot0;
uniform float ambientstrength;
uniform vec3 ambientcolor;
uniform vec3 lightpos;
uniform vec3 lightcolor;
uniform vec3 viewpos;
uniform mat4 invertedmodel;

out vec4 gl_FragColor;
void main(void){
	vec3 ambient = ambientcolor * vec3(ambientstrength);
	
	vec3 normal = normalize(mat3(invertedmodel) * out_normal);
	vec3 lightdir = normalize(lightpos - out_position);
	vec3 diffusion = lightcolor * max(dot(normal, lightdir), 0.0f);

	vec4 lighting = vec4(ambient + diffusion, 1.0f);
	gl_FragColor= vec4(out_color)*texture2D(textureSlot0, out_uv) * lighting;
}