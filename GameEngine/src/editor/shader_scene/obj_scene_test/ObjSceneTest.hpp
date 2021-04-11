#pragma once
#include <vector>
#include <ext/vector_float2.hpp>
#include <ext/vector_float3.hpp>



#include "GraphicObject.hpp"
#include "RenderView.hpp"
#include "ShaderScene.hpp"
#include "Transform.hpp"

class ObjSceneTest : public ShaderScene, RenderView
{
private:
	GLuint programID;

	GLuint textureID;
	GLuint texture;

	/* mvp for shader */
	GLuint matrixID;

	/* graphic object*/
	GraphicObject gObject;

public:
	ObjSceneTest();
	ObjSceneTest(const ObjSceneTest&) = delete;

	void Init() override;
	void OnReloadFragmentShader() override;
	void OnReloadVertexShader() override;
	void OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp) override;
	void Update(float _dt, glm::mat4 _mvp) override;
	void Clean() override;
	char* GetName()  override;
};
