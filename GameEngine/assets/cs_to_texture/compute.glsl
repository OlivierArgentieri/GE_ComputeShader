#version 430

layout (local_size_x = 32, local_size_y = 32) in;

layout (rgba32f, binding = 0) uniform image2D img_output;
layout(std430, binding = 7) buffer layoutName
{
    float time;
    float delta_time;
	float temp;
};

void main() {
  ivec2 coords = ivec2(gl_GlobalInvocationID);
  

  if(time > 360)
  { time = -360;}
  vec4 pixel;

  // make circular color
  float colorx = sin(time+gl_LocalInvocationID.x); // add LocalInvocationID to display each current workGroup
  float colory = cos(time+gl_LocalInvocationID.y); 

  pixel = vec4(colorx, colory ,.25,1.0);
  
  imageStore(img_output, coords, pixel);
}