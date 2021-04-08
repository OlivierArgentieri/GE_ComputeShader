#include "ObjScene.hpp"


#include "DdsLoader.hpp"
#include "imgui.h"
#include "../../../engine/loaders/obj/ObjLoader.hpp"

ObjScene::ObjScene(Game* _game) : ShaderScene(_game)
{
}

void ObjScene::Init()
{
	transform.SetScale(glm::vec3(1, 1, 1));
	transform.SetPosition(glm::vec3(0, 0, 0));
	transform.SetRotation(45, glm::vec3(1, 1, 0));


	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	/**/
	vertexShader.LoadShader("assets/obj/TransformVertexShader.vertexshader", GL_VERTEX_SHADER);
	framgentShader.LoadShader("assets/obj/TextureFragmentShader.fragmentshader", GL_FRAGMENT_SHADER);
	
	vertexShader.CompileShader();
	framgentShader.CompileShader();

	vertexShader.CreateShaderProgram();

	programID = vertexShader.GetProgramID();

	framgentShader.CreateShaderProgram(programID); // same program for both shader
	

	matrixID = glGetUniformLocation(programID, "MVP");

	/* Texture */
	// Load the texture
	DdsLoader::LoadFile("assets/obj/uvChecker.dds", texture);

	// Get a handle for our "myTextureSampler" uniform
	textureID = glGetUniformLocation(programID, "myTextureSampler");

	/* load obj file */
	bool res = ObjLoader::Load("assets/obj/cube.obj", vertices, uvs, normals);
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

}

void ObjScene::ReloadVertexShader()
{
	/*glDetachShader(vertexShader.GetProgramID(), vertexShader.GetShaderID());
	glDetachShader(framgentShader.GetProgramID(), framgentShader.GetShaderID());
	glDeleteShader(vertexShader.GetShaderID());
	glDeleteShader(framgentShader.GetShaderID());*/
	
	//vertexShader.LoadShader("assets/obj/TransformVertexShader.vertexshader", GL_VERTEX_SHADER);
	//framgentShader.LoadShader("assets/obj/TextureFragmentShader.fragmentshader", GL_FRAGMENT_SHADER);

	vertexShader.CompileShader();
	framgentShader.CompileShader();

	vertexShader.CreateShaderProgram();

	programID = vertexShader.GetProgramID();

	framgentShader.CreateShaderProgram(programID); // same program for both shader
	LOG(Info) << "OK";

}

void ObjScene::OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp)
{
	//framgentShader.Use(); // todo static method
	glUseProgram(programID);

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


	ImGui::Begin("Settings");
	if(ImGui::Button("Reload Shaders "))
	{
		ReloadVertexShader();
	}

	char* str1 = &vertexShader.shaderBuffer[0];
	
	ImGui::InputTextMultiline("vertexShader", str1, vertexShader.shaderBuffer.length()*2, ImVec2(800, 500), ImGuiInputTextFlags_Multiline);
	ImGui::End(); 
	
	
}

void ObjScene::Update(float _dt, glm::mat4 _mvp)
{
	Render(_dt, _mvp, GetName().c_str());
}


void ObjScene::Clean()
{
}

string ObjScene::GetName()
{
	return "Obj Scene";
}
