#version 430

layout (local_size_x = 512, local_size_y = 1) in;

float PHI = 1.61803398874989484820459;   
float GRAVITY = -0.08f;

float gold_noise(in vec2 xy, in float seed){
       return fract(tan(distance(xy*PHI, xy)*seed)*xy.x);
}

layout(std430, binding = 6) buffer particlesBuffer
{
    vec3 p[512];
    //float debug[512];
	float delta_time;

};

void main() {
  float time;
  float noise;

  int index = int(gl_LocalInvocationID.x);

  // 				1+ to avoid noise at (0,0) 
  noise=gold_noise(1+gl_LocalInvocationID.xy, gl_LocalInvocationID.x+delta_time);
  if(p[index].y < -5)
	p[index].y = 10;
  p[index].y += GRAVITY * noise;
}
