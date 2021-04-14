#version 330 core

uniform vec3 incolor = vec3(1,0,0);
out vec3 color;

uniform sampler2D csRayTracingTexture;

void main(){
	//color = vec3(1,1,1);
	color = incolor;
}