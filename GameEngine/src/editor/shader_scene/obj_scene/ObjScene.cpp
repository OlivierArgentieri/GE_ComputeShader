#include "ObjScene.hpp"


#include "DdsLoader.hpp"
#include "imgui.h"
#include "../../../engine/loaders/obj/ObjLoader.hpp"

ObjScene::ObjScene() : programID(0), vertexbuffer(0), uvbuffer(0), textureID(0), texture(0), matrixID(0)
{
}

ObjScene::~ObjScene()
{
	delete ssbo_data;
}

void ObjScene::Init()
{
	transform.SetScale(glm::vec3(1, 1, 1));
	transform.SetPosition(glm::vec3(0, 0, 0));
	
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

	/* Texture
	// Load the texture*/
	//DdsLoader::LoadFile("assets/obj/uvChecker.dds", texture);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 640, 480, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	/** SSBO see  */
	// todo see : https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object

	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ssbo_data), ssbo_data, GL_DYNAMIC_READ); //sizeof(data) only works for statically sized C/C++ arrays.
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind
	
	// Get a texture from shader
	textureID = glGetUniformLocation(programID, "myTextureSampler");

	/* load obj file */
	bool res = ObjLoader::Load("assets/obj/cube.obj", vertices, uvs, normals);
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	/** Compute Shader */
	computeShader.LoadShader("assets/obj/base.computeshader", GL_COMPUTE_SHADER);
	computeShader.CompileShader();
	computeShader.CreateShaderProgram();

}

void ObjScene::OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp)
{
	
	ssbo_data->test = _dt;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ssbo_data), ssbo_data, GL_DYNAMIC_READ); //sizeof(data) only works for statically sized C/C++ arrays.
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	
	/** Use compute shader */
	Shader::Use(computeShader.GetProgramID());
	// texture is our output
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute(40, 30, 30);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	
	//framgentShader.Use(); // todo static method
	Shader::Use(fragmentShader.GetProgramID());

	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &_mvp[0][0]);
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(textureID, 0);


	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,(void*)0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	//ImGui::Text("Shader Text");               // Display some text (you can use a format strings too)
	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	glBindTexture(GL_TEXTURE_2D, 0);
}



void ObjScene::OnReloadFragmentShader()
{
	fragmentShader.CompileShader();

	vertexShader.CreateShaderProgram();
	programID = fragmentShader.GetProgramID();

	vertexShader.CreateShaderProgram(programID); // same program for both shader
}

void ObjScene::OnReloadVertexShader()
{
	vertexShader.CompileShader();

	vertexShader.CreateShaderProgram();
	programID = vertexShader.GetProgramID();

	fragmentShader.CreateShaderProgram(programID); // same program for both shader
}

void ObjScene::OnReloadComputeShader()
{
	computeShader.CompileShader();
	computeShader.CreateShaderProgram();
	computeShader.CreateShaderProgram();

	
}

void ObjScene::Update(float _dt, glm::mat4 _mvp)
{
	Render(_dt, _mvp, GetName());
	UpdateSettingsUI();
}



void ObjScene::Clean()
{
}


 char* ObjScene::GetName() 
{
	return "Obj Scene";
}