#version 330 core

// Input
layout(location = 0) in vec3 inVecPosition;
layout(location = 1) in vec2 inUV;

// Output
out vec2 UV;

uniform mat4 MVP;

void main(){
	gl_Position =  MVP * vec4(inVecPosition,1);
	UV = inUV;
}