#pragma once
#include <vector>
#include <ext/vector_float2.hpp>
#include <ext/vector_float3.hpp>



#include "GraphicObject.hpp"
#include "RenderTexture.hpp"
#include "RenderView.hpp"
#include "ShaderScene.hpp"
#include "Transform.hpp"


class RayTracingSecond : public ShaderScene, RenderView, RenderTexture
{
	struct SsboData
	{
		//glm::vec4 vertices[1024];
		float temp=0;
	};

	struct Particle
	{
		glm::vec4 pos;
		glm::vec4 vel;
		glm::vec4 acc;
	};

	std::vector<Particle> particles;
private:
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;

	GLuint programID;

	GLuint vao;
	GLuint vbo;

	/* mvp for shader */
	GLuint matrixID;

	/* SSBO */
	SsboData* ssbo_data = new SsboData();
	GLuint ssbo;
	GLuint outTextureID;
	GLuint outTexture;

	GraphicObject gObject;

protected:
	void OnReloadFragmentShader() override;
	void OnReloadVertexShader() override;
	void OnReloadComputeShader() override;
	GLuint GetTextureToRender() override { return outTexture;}
public:
	RayTracingSecond();
	RayTracingSecond(const RayTracingSecond&) = delete;
	~RayTracingSecond();
	void Init() override;
	void OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp) override;
	
	void Update(float _dt, glm::mat4 _mvp) override;
	void Clean() override;
	char* GetName()  override ; 
};
