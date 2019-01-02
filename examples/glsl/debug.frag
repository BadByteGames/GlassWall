#version 130

in vec4 out_color;

out vec4 gl_FragColor;
void main(void){
	gl_FragColor= vec4(out_color);
}