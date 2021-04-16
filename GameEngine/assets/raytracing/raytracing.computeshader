#version 430

layout (local_size_x = 32, local_size_y = 32) in;

layout (rgba32f, binding = 0) uniform image2D img_output;

// ====== utils ====== //
float length_squared(vec3 _inVec)
{
	return _inVec[0]*_inVec[0] + _inVec[1] * _inVec[1] + _inVec[2] * _inVec[2];
}

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec3 rand()
{
	return vec3(rand(vec2(0.0,1.0)), rand(vec2(0.0,1.0)), rand(vec2(0.0,1.0)));
}

vec3 random_in_unit_sphere()
{
	while(true)
	{
		vec3 _p = rand();
		if(length_squared(_p)>=1) continue;
		return _p;
	}
}

float clamp(float _in, float _min, float _max)
{
	if(_in > _max) return _max;
	if(_in < _min) return _min;
	
	return _in;
}

vec3 write_color(vec3 _pixelColor, int _samplePixel)
{
	float _r = _pixelColor.x;
	float _g = _pixelColor.y;
	float _b = _pixelColor.z;
	
	float _scale = 1.0/ _samplePixel;
	_r = sqrt(_scale *_r);
	_g = sqrt(_scale *_g);
	_b = sqrt(_scale *_b);
	
	return vec3(clamp(_r, 0.0, 0.999), clamp(_g, 0.0, 0.999), clamp(_b, 0.0, 0.999));
}



struct ray
{
    vec3 origin;
    vec3 direction;
	
	vec3 at(float t){return origin + t * direction;}
};

struct hitRecord
{
	vec3 p;
	vec3 normal;
	float t;
	bool frontFace;
	
	void setFaceNormal(ray _r, vec3 _outwardNormal)
	{
		frontFace = dot(_r.direction, _outwardNormal) <0;
		normal = frontFace ? _outwardNormal : - _outwardNormal;
	}
};


bool hit_sphere(vec3 _center, float _radius, inout ray _r, float _tMin, float _tMax, inout hitRecord _rec)
{
	vec3 _oc = _r.origin - _center;
	float _a = length_squared(_r.direction);
	float _halfB = dot(_oc, _r.direction);
	float _c = length_squared(_oc) - _radius * _radius;
	
	float _discriminant = _halfB*_halfB - _a*_c;
	if(_discriminant < 0) return false;
	
	float _sqrtd = sqrt(_discriminant);
	float _root = (-_halfB - _sqrtd)/_a;
	if(_root < _tMin || _tMax < _root) 
	{
		_root = (-_halfB + _sqrtd)/_a;
		if(_root < _tMin || _tMax < _root)
			return false;
	}
	
	_rec.t = _root;
	_rec.p = _r.at(_rec.t);
	vec3 _outwardNormal = (_rec.p - _center) / _radius;
	_rec.setFaceNormal(_r, _outwardNormal);
	return true;
}


struct sphere
{
	vec3 position;
	float radius;
};

struct camera
{
	float aspect_ratio;
	float viewport_height;
	float viewport_width;
	float focal_length;
	
	vec3 origin;
	vec3 horizontal;
	vec3 vertical;
	vec3 lower_left_corner;
	
	ray get_ray(float _u, float _v) 
	{
		return ray(origin, lower_left_corner + _u*horizontal + _v*vertical - origin);
	}
};

// ====== GLOBAL METHODS ====== //

bool hit_objects(ray _r, float _tMin, float _tMax, inout hitRecord _rec, sphere objects[2])
{
	hitRecord _tempRec;
	bool _hitAnything = false;
	float _closestSoFar = _tMax;
	
	for(int _i=0; _i < 2; _i++)
	{
		if(hit_sphere(objects[_i].position, objects[_i].radius, _r, _tMin, _closestSoFar, _tempRec))
		{
			_hitAnything = true;
			_closestSoFar = _tempRec.t;
			_rec = _tempRec;
		}
	}
	return _hitAnything;
}

vec3 ray_color(ray _r, sphere _objects[2])
{
	hitRecord _rec;
	float _infinity = 9999999.0;
	
	
	if(hit_objects(_r, 0, _infinity, _rec, _objects))
	{
		vec3 _target = _rec.p + _rec.normal + random_in_unit_sphere();
		return 0.5 * (_rec.normal + vec3(1,1,1));
	}
	vec3 _unitDirection = _r.direction/length(_r.direction);
	float _t = 0.5*(_unitDirection.y + 1.0);
	return (1.0-_t)*vec3(1.0,1.0,1.0) + _t*vec3(0.5,0.7,1.0);
}


// ====== SSBO ====== //
layout(std430, binding = 5) buffer layoutName
{
	sphere test_array[2];
    float delta_time;
	float time;
};


void main() {
  time += delta_time;
  
  if(time > 360) time=0;

  test_array[0].position.y = sin(time*0.1)*delta_time; // just to test with movement 
  
  
  // image
  float aspect_ratio = 16.0/9.0;
  int image_width = 400;
  int image_height = int(image_width/aspect_ratio);
  int samples_per_pixel = 100;
  
  // camera
  camera cam;
  cam.aspect_ratio = 16.0/9.0;
  cam.viewport_height = 2.0;
  cam.viewport_width = cam.aspect_ratio * cam.viewport_height;
  cam.focal_length = 1.0;

  cam.origin = vec3(0,0,0);
  cam.horizontal = vec3(cam.viewport_width, 0.0, 0.0);
  cam.vertical = vec3(0.0, cam.viewport_height, 0.0);
  cam.lower_left_corner = cam.origin - cam.horizontal/2 - cam.vertical/2 - vec3(0,0,cam.focal_length);

  // render
  ivec2 dims = imageSize(img_output);
  //uvec3 current_pixel = gl_LocalInvocationID * gl_WorkGroupID;
  uvec3 current_pixel = gl_GlobalInvocationID.xyz;
  ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
  vec3 color = vec3(0,0,0);
  
  for(int _i=0; _i<samples_per_pixel; _i++)
  {
	float u = (current_pixel.x + rand(vec2(0.0,1.0)*1.0)) / (dims.x * 1.0f);
    float v = (current_pixel.y + rand(vec2(0.0,1.0)*1.0)) / (dims.y *1.0f);
    ray r = cam.get_ray(u, v);
	color += ray_color(r, test_array);
  }
  
  color = write_color(color, samples_per_pixel);
  
  vec4 pixel = vec4(color.x, color.y, color.z, 1.0);
  
  imageStore(img_output, pixel_coords, pixel);
}