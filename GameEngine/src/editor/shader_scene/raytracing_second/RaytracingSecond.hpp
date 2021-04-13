#pragma once
#include <vector>
#include <ext/vector_float2.hpp>
#include <ext/vector_float3.hpp>

#include "GraphicObject.hpp"
#include "RenderTexture.hpp"
#include "RenderView.hpp"
#include "ShaderScene.hpp"
#include "Transform.hpp"


class RayTracingSecond : public ShaderScene, RenderView
{
	struct Particle
	{
		glm::vec4 pos;
		glm::vec4 vel;
	};
	
	struct SsboData
	{
		glm::vec4 vertices[512];
		float debug[512];
		float delta_time=0;
		float noise=0;
		float temp=0;
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
	glm::vec3 testColor = glm::vec3(0.45f, 0.55f, 0.60f);
	GLint color_location;

protected:
	void OnReloadFragmentShader() override;
	void OnReloadVertexShader() override;
	void OnReloadComputeShader() override;
	void UpdateSettingsUI() override;
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
