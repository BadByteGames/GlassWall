#version 130

in vec4 out_color;
in vec2 out_uv;

uniform sampler2D textureSlot0;

out vec4 gl_FragColor;
void main(void){
	gl_FragColor= vec4(out_color)*texture2D(textureSlot0, out_uv);
}