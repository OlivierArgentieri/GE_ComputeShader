#pragma once
#include "ShaderScene.hpp"

class SampleScene : public ShaderScene
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
	SampleScene(const SampleScene& _sampleScene) = delete;

	void Init() override;
	void Update() override;
	void Clean() override;

private:
	GLuint quadIBO;
	GLuint quadVBO;
	GLuint quadVAO;
	GLuint quadTextureID;

	// vertex - fragment shader pid
	GLuint programID;
};
