#version 330 core

in vec2 UV;
out vec3 color;

uniform sampler2D csRayTracingTexture;

void main(){
	color = texture(csRayTracingTexture, UV).rgb;
}