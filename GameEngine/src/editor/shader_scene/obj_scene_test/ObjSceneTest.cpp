#include "ObjSceneTest.hpp"


#include "DdsLoader.hpp"
#include "imgui.h"
#include "../../../engine/loaders/obj/ObjLoader.hpp"

ObjSceneTest::ObjSceneTest() : programID(0), textureID(0), texture(0), matrixID(0)
{
}

void ObjSceneTest::Init()
{
	transform.SetScale(glm::vec3(1, 1, 1));
	transform.SetPosition(glm::vec3(0, 0, 0));
	pivot = glm::vec3(1, 1, 0);
	angle = glm::radians(90.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	vertexShader.LoadShader("assets/obj/Transform.vertexshader", GL_VERTEX_SHADER);
	fragmentShader.LoadShader("assets/obj/Texture.fragmentshader", GL_FRAGMENT_SHADER);
	
	vertexShader.CompileShader();
	fragmentShader.CompileShader();

	vertexShader.CreateShaderProgram();

	programID = vertexShader.GetProgramID();

	fragmentShader.CreateShaderProgram(programID); // same program for both shader
	
	matrixID = glGetUniformLocation(programID, "MVP");

	/* Load Texture */
	DdsLoader::LoadFile("assets/obj/UVChecker.dds", texture);

	// Get a handle for our "myTexture" uniform
	textureID = glGetUniformLocation(programID, "myTexture");
	
	gObject.LoadFromFile("assets/obj/cube.obj");
	gObject.ComputeBuffers();

}

void ObjSceneTest::OnReloadFragmentShader()
{
	fragmentShader.CompileShader();

	vertexShader.CreateShaderProgram();
	programID = fragmentShader.GetProgramID();

	vertexShader.CreateShaderProgram(programID); // same program for both shader
	
}

void ObjSceneTest::OnReloadVertexShader()
{
	vertexShader.CompileShader();

	vertexShader.CreateShaderProgram();
	programID = vertexShader.GetProgramID();
	
	fragmentShader.CreateShaderProgram(programID); // same program for both shader
}

void ObjSceneTest::OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp)
{
	Shader::Use(fragmentShader.GetProgramID());

	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &_mvp[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(textureID, 0);
	

	gObject.Draw();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void ObjSceneTest::Update(float _dt, glm::mat4 _mvp)
{
	Render(_dt, _mvp, GetName());
	UpdateSettingsUI(_dt);
}

void ObjSceneTest::Clean()
{
}

char * ObjSceneTest::GetName()
{
	return "Obj Scene test";
}
