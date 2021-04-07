#include "ObjScene.hpp"

#include "../../../engine/loaders/obj/obj_loader.h"

void ObjScene::Init()
{
	// test load obj
	glDepthFunc(GL_LESS);
	
	bool res = loadOBJ("assets/obj/cube.obj", vertices, uvs, normals);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	char* _vertexShaderBuffer = 0;
	char* _fragmentShaderBuffer = 0;
	
	vertexShader.LoadShader("assets/obj/TransformVertexShader.vertexshader", GL_VERTEX_SHADER);
	framgentShader.LoadShader("assets/obj/TextureFragmentShader.fragmentshader", GL_FRAGMENT_SHADER);
	
	vertexShader.CompileShader(_vertexShaderBuffer);
	framgentShader.CompileShader(_fragmentShaderBuffer);

	vertexShader.CreateShaderProgram();

	programID = vertexShader.GetProgramID();

	framgentShader.CreateShaderProgram(programID); // same program for both shader

	/* make buffer for obj */
	
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	
}

void ObjScene::Update()
{
	framgentShader.Use(); // todo static method

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
}

void ObjScene::Clean()
{
	
}

string ObjScene::GetName()
{
	return "Obj Scene";
}