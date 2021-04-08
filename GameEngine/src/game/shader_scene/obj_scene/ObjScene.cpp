#include "ObjScene.hpp"

#include "../../../engine/loaders/obj/obj_loader.h"

void ObjScene::Init()
{
	transform.SetScale(glm::vec3(1, 1, 1));
	transform.SetPosition(glm::vec3(0, 0, 0));
	transform.SetRotation(45, glm::vec3(1, 1, 0));

	
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

	matrixID = glGetUniformLocation(programID, "MVP");
	/* Texture */
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	
	/* make buffer for obj */
	
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	
}

void ObjScene::Update(float _dt, glm::mat4 _mvp)
{
	//framgentShader.Use(); // todo static method
	glUseProgram(programID);

	
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &_mvp[0][0]);
	
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
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void ObjScene::Clean()
{
}

string ObjScene::GetName()
{
	return "Obj Scene";
}
