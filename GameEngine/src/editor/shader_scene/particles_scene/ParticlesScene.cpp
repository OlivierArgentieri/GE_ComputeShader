#include "ParticlesScene.hpp"

#include <gtc/type_ptr.inl>



#include "DdsLoader.hpp"
#include "imgui.h"
#include "../../../engine/loaders/obj/ObjLoader.hpp"

const unsigned int ParticlesScene::NB_PARTICLES;

ParticlesScene::ParticlesScene() : programID(0), vao(0), vbo(0), outTextureID(0), outTexture(0), matrixID(0)
{
}

ParticlesScene::~ParticlesScene()
{
	delete ssbo_data;
}

void ParticlesScene::Init()
{
	transform.SetScale(glm::vec3(1, 1, 1));
	transform.SetPosition(glm::vec3(0, 0, 0));
	pivot = glm::vec3(0, 1, 0);
	angle = 0;

	//create vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/** create vbo */
	glGenBuffers(1, &vbo);

	/* load shaders  */
	vertexShader.LoadShader("assets/particles_scene/vert.glsl", GL_VERTEX_SHADER);
	fragmentShader.LoadShader("assets/particles_scene/frag.glsl", GL_FRAGMENT_SHADER);

	vertexShader.CompileShader();
	fragmentShader.CompileShader();

	vertexShader.CreateShaderProgram();

	programID = vertexShader.GetProgramID();

	fragmentShader.CreateShaderProgram(programID); // same program for both shader
	color_location = glGetUniformLocation(fragmentShader.GetProgramID(), "incolor");


	GLint position_attribute = glGetAttribLocation(vertexShader.GetProgramID(), "inVecPosition");
	glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(position_attribute);
	matrixID = glGetUniformLocation(programID, "MVP");


	/** SSBO  */
	// see : https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SsboData), ssbo_data, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // clear


	/* load obj file
	gObject.LoadFromFile("assets/obj/cube.obj");
	gObject.ComputeBuffers(); */

	/** Compute Shader */
	computeShader.LoadShader("assets/particles_scene/compute.glsl", GL_COMPUTE_SHADER);
	computeShader.CompileShader();
	computeShader.CreateShaderProgram();
	
	glDispatchCompute(1, 1, 1); // (1,1,1) because : 1 * 512 : define in cs to generate 512 particles.

	/** Init particles */
	for (int i = 0; i < NB_PARTICLES; i++)
	{
		ssbo_data->vertices[i] = glm::vec4(glm::mix(-2.7, 2.7, ((i * 1.0f) / NB_PARTICLES)), 2, 0, 0);
	}
}

void ParticlesScene::OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp)
{
	//ssbo_data->time += _dt;
	ssbo_data->delta_time = _dt;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(SsboData), ssbo_data);

	/** Use compute shader */
	Shader::Use(computeShader.GetProgramID());
	glDispatchCompute(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);

	/** debug value of shader in console */
	int _index = glGetProgramResourceIndex(computeShader.GetProgramID(), GL_SHADER_STORAGE_BLOCK, "particlesBuffer");
	if (_index == GL_INVALID_INDEX) return;


	glShaderStorageBlockBinding(computeShader.GetProgramID(), _index, 6);
	memcpy(ssbo_data, glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(SsboData), GL_MAP_READ_BIT), sizeof(SsboData));
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	if (!ssbo_data) return;

	vertices.clear();
	for (int i = 0; i < NB_PARTICLES; i++)
	{
		vertices.emplace_back(ssbo_data->vertices[i].x, ssbo_data->vertices[i].y, ssbo_data->vertices[i].z);
	}
	/* Bind vbo */
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	/** use fragment/vertex shader */
	Shader::Use(fragmentShader.GetProgramID());
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &_mvp[0][0]);
	glUniform3fv(color_location, 1, &testColor[0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glPointSize(5);
	glDrawArrays(GL_POINTS, 0, NB_PARTICLES);/**/
}



void ParticlesScene::OnReloadFragmentShader()
{
	fragmentShader.CompileShader();

	vertexShader.CreateShaderProgram();
	programID = fragmentShader.GetProgramID();

	vertexShader.CreateShaderProgram(programID); // same program for both shader
}

void ParticlesScene::OnReloadVertexShader()
{
	vertexShader.CompileShader();

	vertexShader.CreateShaderProgram();
	programID = vertexShader.GetProgramID();

	fragmentShader.CreateShaderProgram(programID); // same program for both shader
}

void ParticlesScene::OnReloadComputeShader()
{
	computeShader.CompileShader();
	computeShader.CreateShaderProgram();
}

void ParticlesScene::UpdateSettingsUI()
{
	ShaderScene::UpdateSettingsUI();
	ImGui::ColorEdit3("test color", &testColor[0]);

	/**/if (!ssbo_data) return;
	static string _temp;
	for (int i = 0; i < NB_PARTICLES; i++)
	{
		_temp = "particuleID: " + std::to_string(i);
		ImGui::InputFloat3(_temp.c_str(), &ssbo_data->vertices[i].x);
	}

}

void ParticlesScene::Update(float _dt, glm::mat4 _mvp)
{
	Render(_dt, _mvp, GetName());
	UpdateSettingsUI();
}



void ParticlesScene::Clean()
{
}


char* ParticlesScene::GetName()
{
	return "Particles Scene";
}