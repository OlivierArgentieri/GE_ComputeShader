#version 430

layout (local_size_x = 1024, local_size_y = 1) in;

float PHI = 1.61803398874989484820459;   
float GRAVITY = -0.08f;

float gold_noise(in vec2 xy, in float seed){
       return fract(tan(distance(xy*PHI, xy)*seed)*xy.x);
}

layout(std430, binding = 6) buffer particlesBuffer
{
    vec3 p[1024];
    //float debug[1024];
	float cursor_x;
	float cursor_y;
	float delta_time;
};

void main() {
  float time;
  float noise;
  
  int index = int(gl_LocalInvocationID.x);
  
  // 				1+ to avoid noise at (0,0) 
  noise=gold_noise(1+gl_LocalInvocationID.xy,index+delta_time);
  vec3 target = vec3(cursor_x,-cursor_y ,0);

  vec3 dir = p[index] - target*0.2;
  //debug[index] = cursor_x;
  p[index] -= dir * noise ; 

}
