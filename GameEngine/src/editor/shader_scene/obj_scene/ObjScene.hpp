#pragma once
#include <vector>
#include <ext/vector_float2.hpp>
#include <ext/vector_float3.hpp>


#include "RenderView.hpp"
#include "ShaderScene.hpp"
#include "Transform.hpp"


class ObjScene : public ShaderScene, RenderView
{
	struct SsboData
	{
		glm::vec4 test[6];
		//float time = 0;
		//float delta_time = 0;
	};

private:
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;

	GLuint programID;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	
	GLuint baseTexture;
	GLuint baseTextureID;

	/* mvp for shader */
	GLuint matrixID;

	/* SSBO */
	SsboData* ssbo_data = new SsboData();
	GLuint ssbo;
	GLuint textureID;
	GLuint texture;

protected:
	void OnReloadFragmentShader() override;
	void OnReloadVertexShader() override;
	void OnReloadComputeShader() override;

public:
	ObjScene();
	ObjScene(const ObjScene&) = delete;
	~ObjScene();
	void Init() override;
	void OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp) override;
	
	void Update(float _dt, glm::mat4 _mvp) override;
	void Clean() override;
	char* GetName()  override ; 
};
