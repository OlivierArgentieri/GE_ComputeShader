#include "Raytracing.hpp"


#include "DdsLoader.hpp"
#include "imgui.h"
#include "../../../engine/loaders/obj/ObjLoader.hpp"

// todo see : https://antongerdelan.net/opengl/compute.html

RayTracing::RayTracing() : matrixID(0), programID(0), inTexture(0), outTexture(0), textureID(0)
{
}

RayTracing::~RayTracing()
{
	delete ssbo_data;
}

void RayTracing::Init()
{
	transform.SetScale(glm::vec3(1, 1, 1));
	transform.SetPosition(glm::vec3(0, 0, 0));
	pivot = glm::vec3(1, 1, 0);
	angle = glm::radians(90.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	/**/
	vertexShader.LoadShader("assets/raytracing/Transform.vertexshader", GL_VERTEX_SHADER);
	fragmentShader.LoadShader("assets/raytracing/Texture.fragmentshader", GL_FRAGMENT_SHADER);

	vertexShader.CompileShader();
	fragmentShader.CompileShader();

	vertexShader.CreateShaderProgram();

	programID = vertexShader.GetProgramID();

	fragmentShader.CreateShaderProgram(programID); // same program for both shader

	matrixID = glGetUniformLocation(programID, "MVP");

	/** Texture */
	// dimensions of the image
	
	glGenTextures(1, &outTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, outTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, FrameBufferObject::SIZE_X_VIEWPORT, FrameBufferObject::SIZE_Y_VIEWPORT, 0, GL_RGBA, GL_FLOAT,nullptr);
	
	/** SSBO  */	
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SsboData), ssbo_data, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // clear

	/** load obj file */
	gObject.LoadFromFile("assets/raytracing/cube.obj");
	gObject.ComputeBuffers();

	/** Compute Shader */
	computeShader.LoadShader("assets/raytracing/raytracing.computeshader", GL_COMPUTE_SHADER);
	computeShader.CompileShader();
	computeShader.CreateShaderProgram();
	textureID = glGetUniformLocation(programID, "csRayTracingTexture");

	/** load */
	glBindImageTexture(0, outTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	/** Determining the work group size */
	glDispatchCompute(32, 30, 1); // 1024 768 
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

void RayTracing::OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp)
{
	ssbo_data->time += _dt;
	ssbo_data->delta_time = _dt;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(SsboData), ssbo_data);

	/** Use compute shader */
	Shader::Use(computeShader.GetProgramID());
	glBindImageTexture(0, outTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute(32, 30, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	/** update SSBO value  */
	int _index = glGetProgramResourceIndex(computeShader.GetProgramID(), GL_SHADER_STORAGE_BLOCK, "layoutName");
	if (_index != GL_INVALID_INDEX)
	{
		glShaderStorageBlockBinding(computeShader.GetProgramID(), _index, 5);
		memcpy(ssbo_data, glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(SsboData), GL_MAP_READ_BIT), sizeof(SsboData));
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		/**/if (ssbo_data)
		{
			LOG(Info) << ssbo_data->temp;
		}
	}

	Shader::Use(fragmentShader.GetProgramID());
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &_mvp[0][0]);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, outTexture);
	glUniform1i(textureID, 0);

	gObject.Draw();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

void RayTracing::OnReloadFragmentShader()
{
	fragmentShader.CompileShader();

	vertexShader.CreateShaderProgram();
	programID = fragmentShader.GetProgramID();

	vertexShader.CreateShaderProgram(programID); // same program for both shader
}

void RayTracing::OnReloadVertexShader()
{
	vertexShader.CompileShader();

	vertexShader.CreateShaderProgram();
	programID = vertexShader.GetProgramID();

	fragmentShader.CreateShaderProgram(programID); // same program for both shader
}

void RayTracing::OnReloadComputeShader()
{
	computeShader.CompileShader();
	computeShader.CreateShaderProgram();
}

void RayTracing::Update(float _dt, glm::mat4 _mvp)
{
	RenderView::Render(_dt, _mvp, GetName());
	RenderTexture::Render();
	UpdateSettingsUI(_dt);
}

void RayTracing::Clean()
{
}

char* RayTracing::GetName()
{
	return "Test raytracing";
}