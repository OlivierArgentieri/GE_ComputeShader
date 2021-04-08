#pragma once
#include <vector>
#include <ext/vector_float2.hpp>
#include <ext/vector_float3.hpp>


#include "RenderView.hpp"
#include "ShaderScene.hpp"
#include "Transform.hpp"

class ObjScene : public ShaderScene, RenderView
{
private:
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; // Won't be used at the moment.

	GLuint programID;
	Shader vertexShader;
	Shader framgentShader;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	
	GLuint textureID;
	GLuint texture;

	/* mvp for shader */
	GLuint matrixID;
	
public:
	ObjScene(Game* _game);
	ObjScene(const ObjScene&) = delete;

	void Init() override;
	void ReloadVertexShader();
	void OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp) override;
	void Update(float _dt, glm::mat4 _mvp) override;
	void NotUpdate(float _dt, glm::mat4 _mvp);
	void Clean() override;
	string GetName() override;
};
