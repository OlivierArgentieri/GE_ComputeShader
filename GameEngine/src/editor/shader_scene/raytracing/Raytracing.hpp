#pragma once
#include "RenderView.hpp"
#include "ShaderScene.hpp"
#include "GraphicObject.hpp"
#include "RenderTexture.hpp"


class RayTracing :  public ShaderScene, RenderView, RenderTexture
{

	
	class Sphere 
	{
	private:
		// f/p
		glm::vec3 center;
		float radius;
	
	public:
		Sphere() {}
		Sphere(glm::vec3 _center, double _r) : center(_center), radius(_r) {}
	};
	
	struct SsboData
	{
		//glm::vec4 center=glm::vec4(0,0,-1,0);
		//float radius = 0.5;
		//Hittable* hittables[1];
		//float time = 0;
		//float temp = 0;
		Sphere spheres[2] = {};
		float delta_time = 0;
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
	Sphere testSphere2 = Sphere(glm::vec3(0, -100.5, -1), 100.0f);

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
