#include "ObjSceneTest.hpp"


#include "DdsLoader.hpp"
#include "imgui.h"
#include "../../../engine/loaders/obj/ObjLoader.hpp"

ObjSceneTest::ObjSceneTest() : programID(0), vertexbuffer(0), uvbuffer(0), textureID(0), texture(0), matrixID(0)
{
}

void ObjSceneTest::Init()
{
	transform.SetScale(glm::vec3(1, 1, 1));
	transform.SetPosition(glm::vec3(0, 0, 0));
	transform.SetRotation(45, glm::vec3(1, 1, 0));


	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	/**/
	vertexShader.LoadShader("assets/obj/Transform.vertexshader", GL_VERTEX_SHADER);
	fragmentShader.LoadShader("assets/obj/Texture.fragmentshader", GL_FRAGMENT_SHADER);
	
	vertexShader.CompileShader();
	fragmentShader.CompileShader();

	vertexShader.CreateShaderProgram();

	programID = vertexShader.GetProgramID();

	fragmentShader.CreateShaderProgram(programID); // same program for both shader
	

	matrixID = glGetUniformLocation(programID, "MVP");

	/* Texture */
	// Load the texture
	DdsLoader::LoadFile("assets/obj/UVChecker.dds", texture);

	// Get a handle for our "myTexture" uniform
	textureID = glGetUniformLocation(programID, "myTexture");

	/* load obj file */
	bool res = ObjLoader::Load("assets/obj/cube.obj", vertices, uvs, normals);
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

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
	LOG(Info) << "OK";
}

void ObjSceneTest::OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp)
{
	//framgentShader.Use(); // todo static method
	Shader::Use(fragmentShader.GetProgramID());

	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &_mvp[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(textureID, 0);


	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,(void*)0);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	//ImGui::Text("Shader Text");               // Display some text (you can use a format strings too)
	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void ObjSceneTest::Update(float _dt, glm::mat4 _mvp)
{
	Render(_dt, _mvp, GetName());
	UpdateSettingsUI();
}


void ObjSceneTest::Clean()
{
}


char * ObjSceneTest::GetName()
{
	return "Obj Scene test";
}
