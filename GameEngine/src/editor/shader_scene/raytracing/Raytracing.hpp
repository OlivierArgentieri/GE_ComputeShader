#pragma once
#include "RenderView.hpp"
#include "ShaderScene.hpp"
#include "GraphicObject.hpp"
#include "RenderTexture.hpp"


class RayTracing :  public ShaderScene, RenderView, RenderTexture
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
