#pragma once
#include <vector>
#include <ext/vector_float2.hpp>
#include <ext/vector_float3.hpp>

#include "ShaderScene.hpp"
#include "Transform.hpp"

class ObjScene : public ShaderScene
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


	/* mvp */
	GLuint matrixID;
	glm::mat4 mvp;
	
public:
	ObjScene() = default;
	ObjScene(const ObjScene&) = delete;

	void Init() override;
	void Update(float _dt, glm::mat4 _mvp) override;
	void Clean() override;
	string GetName() override;
};
