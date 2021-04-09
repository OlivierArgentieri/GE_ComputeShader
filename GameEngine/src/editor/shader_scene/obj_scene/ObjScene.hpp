#pragma once
#include <vector>
#include <ext/vector_float2.hpp>
#include <ext/vector_float3.hpp>


#include "RenderView.hpp"
#include "ShaderScene.hpp"
#include "Transform.hpp"
struct SsboData
{
	float test = 0.5f;
};
class ObjScene : public ShaderScene, RenderView
{
private:
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; // Won't be used at the moment.

	GLuint programID;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	
	GLuint textureID;
	GLuint texture;

	/* mvp for shader */
	GLuint matrixID;

	/* SSBO */
	SsboData* ssbo_data = new SsboData();
	GLuint ssbo;

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
