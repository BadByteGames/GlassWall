#version 130

in vec3 in_position;
in vec3 in_color;

out vec3 out_color;
void main(void){
	gl_Position = vec4(in_position, 1.0);

	out_color = in_color;
}