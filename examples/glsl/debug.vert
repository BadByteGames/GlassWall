#version 130

in vec3 in_position;
in vec4 in_color;
in vec2 in_uv;

uniform mat4 mvp;

out vec4 out_color;
out vec2 out_uv;
void main(void){
	gl_Position = mvp * vec4(in_position, 1.0);

	out_color = in_color;
	out_uv = in_uv;
}