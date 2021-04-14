#pragma once
#include <vector>
#include <ext/vector_float2.hpp>
#include <ext/vector_float3.hpp>

#include "GraphicObject.hpp"
#include "RenderTexture.hpp"
#include "RenderView.hpp"
#include "ShaderScene.hpp"
#include "Transform.hpp"


class ParticlesSceneSecond : public ShaderScene, RenderView
{
	const static unsigned int NB_PARTICLES = 1024;
	
	struct SsboData
	{
		//float debug[NB_PARTICLES];
		glm::vec4 vertices[NB_PARTICLES];

		float cursor_x;
		float cursor_y;
		float delta_time=0;
	};

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

private:
	void resetPos();
protected:
	void OnReloadFragmentShader() override;
	void OnReloadVertexShader() override;
	void OnReloadComputeShader() override;
	void UpdateSettingsUI(float _dt) override;

public:
	ParticlesSceneSecond();
	ParticlesSceneSecond(const ParticlesSceneSecond&) = delete;
	~ParticlesSceneSecond();
	void Init() override;
	void OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp) override;
	
	void Update(float _dt, glm::mat4 _mvp) override;
	void Clean() override;
	char* GetName()  override ; 
};
