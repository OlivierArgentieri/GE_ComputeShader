#pragma once
#include "RenderView.hpp"
#include "ShaderScene.hpp"

class SampleScene : public ShaderScene, RenderView
{
	typedef struct _COORDS_ {
		GLfloat x;
		GLfloat y;
		GLfloat s;
		GLfloat t;
	} CoordinatesSet;

	typedef struct _RGBA_ {
		GLfloat Red;
		GLfloat Green;
		GLfloat Blue;
		GLfloat Alpha;
	} RGBAValues;
	
public:
	
	SampleScene();
	SampleScene(const SampleScene&) = delete;

	void Init() override;
	void Update(float _dt, glm::mat4 _mvp) override;
	void Clean() override;
	char* GetName()  override;

private:
	GLuint quadIBO;
	GLuint quadVBO;
	GLuint quadVAO;
	GLuint quadTextureID;

	// vertex - fragment shader pid
	GLuint programID;

	void OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp) override;

	/** === shaders === */
	Shader vertexShader;
	Shader fragmentShader;
	Shader computeShader;
};
