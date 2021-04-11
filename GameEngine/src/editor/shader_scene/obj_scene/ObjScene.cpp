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
	DdsLoader::LoadFile("assets/obj/uvChecker.dds", baseTexture);
	baseTextureID = glGetUniformLocation(programID, "myTexture");

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, FrameBufferObject::SIZE_X_VIEWPORT, FrameBufferObject::SIZE_Y_VIEWPORT, 0, GL_RGBA, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

	/** SSBO  */
	// see : https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object

	/**/glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SsboData), ssbo_data, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // clear

	// use cs buffer as vertices
	//glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	//glBindBuffer(GL_ARRAY_BUFFER, ssbo);


	/* load obj file */
	//bool res = ObjLoader::Load("assets/obj/cube.obj", vertices, uvs, normals);

	/*
	vertices.push_back(glm::vec3(-(transform.scale.x / 2) + transform.position.x, (transform.scale.y / 2) + transform.position.y, -(transform.scale.z / 2) + transform.position.z));
	vertices.push_back(glm::vec3(-(transform.scale.x / 2) + transform.position.x, (transform.scale.y / 2) + transform.position.y, (transform.scale.z / 2) + transform.position.z));
	vertices.push_back(glm::vec3((transform.scale.x / 2) + transform.position.x, (transform.scale.y / 2) + transform.position.y, (transform.scale.z / 2) + transform.position.z));

	vertices.push_back(glm::vec3((transform.scale.x / 2) + transform.position.x, (transform.scale.y / 2) + transform.position.y, (transform.scale.z / 2) + transform.position.z));
	vertices.push_back(glm::vec3((transform.scale.x / 2) + transform.position.x, (transform.scale.y / 2) + transform.position.y, -(transform.scale.z / 2) + transform.position.z));
	vertices.push_back(glm::vec3(-(transform.scale.x / 2) + transform.position.x, (transform.scale.y / 2) + transform.position.y, -(transform.scale.z / 2) + transform.position.z));*/

	uvs.push_back(glm::vec2(0, 0));
	uvs.push_back(glm::vec2(1, 0));
	uvs.push_back(glm::vec2(1, 1));

	uvs.push_back(glm::vec2(1, 1));
	uvs.push_back(glm::vec2(0, 1));
	uvs.push_back(glm::vec2(0, 0));

	if (ssbo_data)
	{
		// convert vec4 -> vec3
		for (glm::vec3 _vec : ssbo_data->test)
		{
			vertices.push_back(_vec);
		}
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &ssbo_data->test[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	}
	/** Compute Shader */
	computeShader.LoadShader("assets/obj/obj.computeshader", GL_COMPUTE_SHADER);
	computeShader.CompileShader();
	computeShader.CreateShaderProgram();
	Shader::Use(computeShader.GetProgramID());
	glDispatchCompute(1, 1, 1);
	//
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

}

void ObjScene::OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp)
{
	//ssbo_data->time += _dt;
	//ssbo_data->delta_time = _dt;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);


	/** Use compute shader */
	Shader::Use(computeShader.GetProgramID());
	glDispatchCompute(1, 1, 1);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SsboData), ssbo_data, GL_DYNAMIC_COPY);

	 // retrieve data from cs //
	int _index = glGetProgramResourceIndex(computeShader.GetProgramID(), GL_SHADER_STORAGE_BLOCK, "layoutName");
	if (_index != GL_INVALID_INDEX)
	{
		glShaderStorageBlockBinding(computeShader.GetProgramID(), _index, 3);
		memcpy(ssbo_data, glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(SsboData), GL_MAP_READ_BIT), sizeof(SsboData));
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		if (ssbo_data)
		{
			vertices.clear();
			for (glm::vec3 _vec : ssbo_data->test)
			{
				vertices.push_back(_vec);
			}
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &ssbo_data->test[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
			glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		}
			//LOG(Info) << ssbo_data->time;
	}

	Shader::Use(fragmentShader.GetProgramID());
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &_mvp[0][0]);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(textureID, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
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