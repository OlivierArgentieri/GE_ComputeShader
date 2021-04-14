#include "CsToTexture.hpp"


#include "DdsLoader.hpp"
#include "imgui.h"
#include "../../../engine/loaders/obj/ObjLoader.hpp"

CsToTexture::CsToTexture() : programID(0), textureID(0), texture(0), matrixID(0)
{
}

CsToTexture::~CsToTexture()
{
	delete ssbo_data;
}

void CsToTexture::Init()
{
	transform.SetScale(glm::vec3(1, 1, 1));
	transform.SetPosition(glm::vec3(0, 0, 0));
	pivot = glm::vec3(1, 1, 0);
	angle = glm::radians(90.0f);
	
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	/**/
	vertexShader.LoadShader("assets/cs_to_texture/vert.glsl", GL_VERTEX_SHADER);
	fragmentShader.LoadShader("assets/cs_to_texture/frag.glsl", GL_FRAGMENT_SHADER);

	vertexShader.CompileShader();
	fragmentShader.CompileShader();

	vertexShader.CreateShaderProgram();

	programID = vertexShader.GetProgramID();

	fragmentShader.CreateShaderProgram(programID); // same program for both shader


	matrixID = glGetUniformLocation(programID, "MVP");

	/** Texture */
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

	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SsboData), ssbo_data, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // clear


	/* load obj file */
	gObject.LoadFromFile("assets/obj/cube.obj");
	gObject.ComputeBuffers();

	/** Compute Shader */
	computeShader.LoadShader("assets/cs_to_texture/compute.glsl", GL_COMPUTE_SHADER);
	computeShader.CompileShader();
	computeShader.CreateShaderProgram();
	textureID = glGetUniformLocation(programID, "mycsTexture");

	
	// texture is our output
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	glDispatchCompute(40, 30, 1); // (40,30,1) because : 32 * 40 = FrameBufferObject::SIZE_X_VIEWPORT  and 32*30 = FrameBufferObject::SIZE_Y_VIEWPORT : 32 is define in cs, on top
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CsToTexture::OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp)
{
	ssbo_data->time += _dt;
	ssbo_data->delta_time = _dt;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(SsboData), ssbo_data);


	/** Use compute shader */
	Shader::Use(computeShader.GetProgramID());
	//glBindTexture(GL_TEXTURE_2D, texture);
	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute(40, 30, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	//glBindTexture(GL_TEXTURE_2D, 0);

	/** update SSBO value  */
	int _index = glGetProgramResourceIndex(computeShader.GetProgramID(), GL_SHADER_STORAGE_BLOCK, "layoutName");
	if (_index != GL_INVALID_INDEX)
	{
		glShaderStorageBlockBinding(computeShader.GetProgramID(), _index, 7);
		memcpy(ssbo_data, glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(SsboData), GL_MAP_READ_BIT), sizeof(SsboData));
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		/*if (ssbo_data) // uncomment if you want debug
		{
			LOG(Info) << ssbo_data->temp;
		}*/
	}

	Shader::Use(fragmentShader.GetProgramID());
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &_mvp[0][0]);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(textureID, 0);

	gObject.Draw();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}



void CsToTexture::OnReloadFragmentShader()
{
	fragmentShader.CompileShader();

	vertexShader.CreateShaderProgram();
	programID = fragmentShader.GetProgramID();

	vertexShader.CreateShaderProgram(programID); // same program for both shader
}

void CsToTexture::OnReloadVertexShader()
{
	vertexShader.CompileShader();

	vertexShader.CreateShaderProgram();
	programID = vertexShader.GetProgramID();

	fragmentShader.CreateShaderProgram(programID); // same program for both shader
}

void CsToTexture::OnReloadComputeShader()
{
	computeShader.CompileShader();
	computeShader.CreateShaderProgram();
}
void CsToTexture::Update(float _dt, glm::mat4 _mvp)
{
	RenderView::Render(_dt, _mvp, GetName());
	RenderTexture::Render();
	UpdateSettingsUI(_dt);
}

void CsToTexture::Clean()
{
}


char* CsToTexture::GetName()
{
	return "Cs To texture";
}