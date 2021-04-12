#pragma once
#include "RenderView.hpp"
#include "ShaderScene.hpp"
#include "GraphicObject.hpp"
#include "RenderTexture.hpp"


class CsToTexture :  public ShaderScene, RenderView, RenderTexture
{
	struct SsboData
	{
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

	/* Graphic object */
	GraphicObject gObject;

protected:
	GLuint GetTextureToRender() override { return texture; }
public:
	CsToTexture();
	CsToTexture(const CsToTexture&) = delete;
	~CsToTexture();

	void Init() override;
	void Update(float _dt, glm::mat4 _mvp) override;
	void Clean() override;
	char* GetName() override;
};
