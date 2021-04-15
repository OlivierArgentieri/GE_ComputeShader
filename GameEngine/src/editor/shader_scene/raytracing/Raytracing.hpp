#pragma once
#include "RenderView.hpp"
#include "ShaderScene.hpp"
#include "GraphicObject.hpp"
#include "RenderTexture.hpp"


class RayTracing :  public ShaderScene, RenderView, RenderTexture
{

	struct ray
	{
		glm::vec3 origin;
		glm::vec3 direction;

		glm::vec3 at(float t) { return origin + t * direction; }
	};
	
	struct HitRecord
	{
		glm::vec3 p;
		glm::vec3 normal;
		float t;
		bool frontFace;

		void SetFaceNormal(ray _r, glm::vec3 _outwardNormal)
		{
			frontFace = dot(_r.direction, _outwardNormal);
			normal = frontFace ? _outwardNormal : -_outwardNormal;
		};
	};

	class Hittable
	{
	public:
		virtual bool hit(ray& _r, float _tMin, float _tMax, HitRecord& _rec) const = 0;
	};
	

	class Sphere : public Hittable
	{
	private:
		// f/p
		glm::vec3 center;
		float radius;

		
		
		static float length_squared(glm::vec3 _inVec)
		{
			return _inVec[0] * _inVec[0] + _inVec[1] * _inVec[1] + _inVec[2] * _inVec[2];
		}

		// hit behaviour
	public:
		Sphere() {}
		Sphere(glm::vec3 _center, double _r) : center(_center), radius(_r) {}
		
		bool hit(ray& _r, float _tMin, float _tMax, HitRecord& _rec) const override
		{
		
			glm::vec3 _oc = _r.origin - center;
			float _a = length_squared(_r.direction);
			float _halfB = dot(_oc, _r.direction);
			float _c = length_squared(_oc) - radius * radius;
			float _discriminant = _halfB * _halfB - _a * _c;
			if (_discriminant < 0) return false;
			float _sqrtd = sqrt(_discriminant);

			float _root = (-_halfB - _sqrtd) / _a;
			if (_root < _tMin || _tMax < _root)
			{
				_root = (-_halfB + _sqrtd) / _a;
				if (_root < _tMin || _tMax < _root) return false;
			}
			_rec.t = _root;
			_rec.p = _r.at(_rec.t);
			glm::vec3 _outwardNormal = (_rec.p - center) / radius;
			_rec.SetFaceNormal(_r, _outwardNormal);
			return true;
		
		}
	};
	
	struct SsboData
	{
		glm::vec4 center=glm::vec4(0,0,-1,0);
		float radius = 0.5;
		//Hittable* hittables[1];
		float time = 0;
		float delta_time = 0;
		float temp = 0;
	};

private:
	void OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp) override;
	void OnReloadFragmentShader() override;
	void OnReloadVertexShader() override;
	void OnReloadComputeShader() override;

private:
	/** mvp for shader */
	GLuint matrixID;
	
	GLuint programID;
	GLuint inTexture;
	GLuint outTexture;
	GLuint locRadius;
	GLuint csBufferIndex;

	/** SSBO */
	SsboData* ssbo_data = new SsboData();
	GLuint ssbo;
	GLuint textureID;

	/** Graphic object */
	GraphicObject gObject;

	/** raytracing */
	int tex_w = 512, tex_h = 512;

	/** test hittable objects */
	Sphere testSphere = Sphere(glm::vec3(0, 0, -1), 0.5);
protected:
	/** for render texture view */
	GLuint GetTextureToRender() override{ return outTexture; }

public:
	RayTracing();
	RayTracing(const RayTracing&) = delete;
	~RayTracing();

	void Init() override;
	void Update(float _dt, glm::mat4 _mvp) override;
	void Clean() override;
	char* GetName() override;
};
