#pragma once
#include "RenderView.hpp"
#include "ShaderScene.hpp"



class CsToTexture :  public ShaderScene, RenderView
{
	struct SsboData
	{
		float time = 0;
		float delta_time = 0;
	};
private:
	void OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp) override;
	void OnReloadFragmentShader() override;
	void OnReloadVertexShader() override;
	void OnReloadComputeShader() override;

	


private:
	/* mvp for shader */
	GLuint matrixID;
	
	GLuint programID;
	GLuint texture;
	GLuint locRadius;
	GLuint csBufferIndex;

	/* SSBO */
	SsboData* ssbo_data = new SsboData();
	GLuint ssbo;
	GLuint textureID;

	/** for load object */
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	GLuint vertexbuffer;
	GLuint uvbuffer;

public:
	CsToTexture();
	CsToTexture(const CsToTexture&) = delete;
	~CsToTexture();

	void Init() override;
	void Update(float _dt, glm::mat4 _mvp) override;
	void Clean() override;
	char* GetName() override;
};
