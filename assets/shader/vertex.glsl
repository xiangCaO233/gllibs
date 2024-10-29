#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 vcolor;

out vec4 color;
void main(){
	gl_Position = vec4(pos,1.0f);
	color = vcolor;
}
