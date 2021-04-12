#include "RaytracingSecond.hpp"


#include "DdsLoader.hpp"
#include "imgui.h"
#include "../../../engine/loaders/obj/ObjLoader.hpp"

RayTracingSecond::RayTracingSecond() : programID(0), vertexbuffer(0), uvbuffer(0), outTextureID(0), outTexture(0), matrixID(0)
{
}

RayTracingSecond::~RayTracingSecond()
{
	delete ssbo_data;
}

void RayTracingSecond::Init()
{
	transform.SetScale(glm::vec3(1, 1, 1));
	transform.SetPosition(glm::vec3(0, 0, 0));
	pivot = glm::vec3(1, 1, 0);
	angle = glm::radians(90.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	/**/
	vertexShader.LoadShader("assets/raytracing_second/Transform.vertexshader", GL_VERTEX_SHADER);
	fragmentShader.LoadShader("assets/raytracing_second/Texture.fragmentshader", GL_FRAGMENT_SHADER);

	vertexShader.CompileShader();
	fragmentShader.CompileShader();

	vertexShader.CreateShaderProgram();

	programID = vertexShader.GetProgramID();

	fragmentShader.CreateShaderProgram(programID); // same program for both shader


	matrixID = glGetUniformLocation(programID, "MVP");

	/** Texture */
	glGenTextures(1, &outTexture);
	glBindTexture(GL_TEXTURE_2D, outTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, FrameBufferObject::SIZE_X_VIEWPORT, FrameBufferObject::SIZE_Y_VIEWPORT, 0, GL_RGBA, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

	/** SSBO  */
	// see : https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object

	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SsboData), ssbo_data, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // clear


	/* load obj file */
	gObject.LoadFromFile("assets/obj/cube.obj");
	gObject.ComputeBuffers();

	/** Compute Shader */
	computeShader.LoadShader("assets/raytracing_second/raytracing_second.computeshader", GL_COMPUTE_SHADER);
	computeShader.CompileShader();
	computeShader.CreateShaderProgram();
	outTextureID = glGetUniformLocation(programID, "mycsTexture");


	// texture is our output
	glBindTexture(GL_TEXTURE_2D, outTexture);
	glBindImageTexture(0, outTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	glDispatchCompute(30, 40, 1); // (40,30,1) because : 32 * 40 = FrameBufferObject::SIZE_X_VIEWPORT  and 32*30 = FrameBufferObject::SIZE_Y_VIEWPORT : 32 is define in cs, on top
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void RayTracingSecond::OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp)
{
	ssbo_data->time += _dt;
	ssbo_data->delta_time = _dt;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(SsboData), ssbo_data);


	/** Use compute shader */
	Shader::Use(computeShader.GetProgramID());
	//glBindTexture(GL_TEXTURE_2D, texture);
	glBindImageTexture(0, outTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute(30, 40, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	//glBindTexture(GL_TEXTURE_2D, 0);

	/** debug value of shader in console */
	// uncomment if you want to test 
	/**/int _index = glGetProgramResourceIndex(computeShader.GetProgramID(), GL_SHADER_STORAGE_BLOCK, "layoutName");
	if (_index != GL_INVALID_INDEX)
	{
		glShaderStorageBlockBinding(computeShader.GetProgramID(), _index, 6);
		memcpy(ssbo_data, glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(SsboData), GL_MAP_READ_BIT), sizeof(SsboData));
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		/*if (ssbo_data)
		{
			LOG(Info) << ssbo_data->temp;
		}*/
	}

	Shader::Use(fragmentShader.GetProgramID());
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &_mvp[0][0]);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, outTexture);
	glUniform1i(outTextureID, 0);

	gObject.Draw();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

}



void RayTracingSecond::OnReloadFragmentShader()
{
	fragmentShader.CompileShader();

	vertexShader.CreateShaderProgram();
	programID = fragmentShader.GetProgramID();

	vertexShader.CreateShaderProgram(programID); // same program for both shader
}

void RayTracingSecond::OnReloadVertexShader()
{
	vertexShader.CompileShader();

	vertexShader.CreateShaderProgram();
	programID = vertexShader.GetProgramID();

	fragmentShader.CreateShaderProgram(programID); // same program for both shader
}

void RayTracingSecond::OnReloadComputeShader()
{
	computeShader.CompileShader();
	computeShader.CreateShaderProgram();
}

void RayTracingSecond::Update(float _dt, glm::mat4 _mvp)
{
	RenderView::Render(_dt, _mvp, GetName());
	RenderTexture::Render();
	UpdateSettingsUI();
}



void RayTracingSecond::Clean()
{
}


char* RayTracingSecond::GetName()
{
	return "Ray Tracing 2";
}