#version 430

layout (local_size_x = 512, local_size_y = 1) in;

//layout (rgba32f, binding = 0) uniform image2D img_output;
// layout (rgba32f, binding = 1) uniform image2D img_input;

float PHI = 1.61803398874989484820459;  // Φ = Golden Ratio   
float GRAVITY = -0.08f;

float gold_noise(in vec2 xy, in float seed){
       return fract(tan(distance(xy*PHI, xy)*seed)*xy.x);
}

layout(std430, binding = 6) buffer particlesBuffer
{
    vec3 p[512];
    //float debug[512];
	float delta_time;
	//float noise;
	//float temp;
};

void main() {
  float time;
  float noise;
  // gl_LocalInvocationID -> where we are in local size (32,32)
  // gl_WorkGroupID -> where we are in WorkGroup size (30,40,1)
  // gl_GlobalInvocationID -> where we are in image (gl_LocalInvocationID * gl_WorkGroupID)
  // temp = 1;
  
  //time += delta_time;
  //temp %=360;
  
  int index = int(gl_LocalInvocationID.x-1);
  
  
  //debug[gl_LocalInvocationID.x]=gl_GlobalInvocationID.x;
  
  //debug = gl_GlobalInvocationID.x;
  //debug = gl_GlobalInvocationID.y;
  
  //debug = gl_WorkGroupID.x;
  //debug = gl_WorkGroupID.y;
  
  //debug = gl_LocalInvocationID.x;
  //debug = gl_LocalInvocationID.y;
  
  //debug = gl_NumWorkGroups.x;
  //debug = gl_NumWorkGroups.y;
  
  //debug = gl_WorkGroupSize.x;
  //debug = gl_WorkGroupSize.y;
  
  
  noise=gold_noise(gl_LocalInvocationID.xy, gl_LocalInvocationID.x+delta_time);
  if(p[index].y < -5)
	p[index].y = 10;
  p[index].y += GRAVITY * noise;
  //temp = gl_NumWorkGroups.x;
  //temp = gl_GlobalInvocationID.x;
  //temp = gl_NumWorkGroups.x;
  //p[index].c = vec4(1., 0., 0., 1.);
}