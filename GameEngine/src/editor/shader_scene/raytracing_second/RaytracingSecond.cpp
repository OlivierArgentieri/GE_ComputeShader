#include "RaytracingSecond.hpp"

#include <gtc/type_ptr.inl>



#include "DdsLoader.hpp"
#include "imgui.h"
#include "../../../engine/loaders/obj/ObjLoader.hpp"

RayTracingSecond::RayTracingSecond() : programID(0), vao(0), vbo(0), outTextureID(0), outTexture(0), matrixID(0)
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
	pivot = glm::vec3(0,1,0);
	angle = 0;

	//create vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	
	/*/#1#/ create vertices#1#
	vertices.push_back(glm::vec3(0,0,0));
	vertices.push_back(glm::vec3(0.5,0,0));
	vertices.push_back(glm::vec3(0.6,0,0));
	vertices.push_back(glm::vec3(0.7,0,0));
	vertices.push_back(glm::vec3(0.8,0,0));
	vertices.push_back(glm::vec3(0.9,0,0));
	vertices.push_back(glm::vec3(1,0,0));*/
	
	/*#1#/ create vbo#1#*/
	glGenBuffers(1, &vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	
	/* load shaders  */
	vertexShader.LoadShader("assets/raytracing_second/Transform.vertexshader", GL_VERTEX_SHADER);
	fragmentShader.LoadShader("assets/raytracing_second/Texture.fragmentshader", GL_FRAGMENT_SHADER);

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
	computeShader.LoadShader("assets/raytracing_second/raytracing_second.computeshader", GL_COMPUTE_SHADER);
	computeShader.CompileShader();
	computeShader.CreateShaderProgram();
	//outTextureID = glGetUniformLocation(programID, "mycsTexture");

	// Shader::Use(computeShader.GetProgramID());
	// texture is our output
	
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glDispatchCompute(1, 1, 1); // (40,30,1) because : 32 * 40 = FrameBufferObject::SIZE_X_VIEWPORT  and 32*30 = FrameBufferObject::SIZE_Y_VIEWPORT : 32 is define in cs, on top
	//glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	for(int i =0; i< 512 ; i++)
	{
		ssbo_data->vertices[i] = glm::vec4(0);
		ssbo_data->debug[i] = -1;
	}

	
}

void RayTracingSecond::OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp)
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
	// uncomment if you want to test /**/
	int _index = glGetProgramResourceIndex(computeShader.GetProgramID(), GL_SHADER_STORAGE_BLOCK, "particlesBuffer");
	if (_index != GL_INVALID_INDEX)
	{
		

		glShaderStorageBlockBinding(computeShader.GetProgramID(), _index, 6);
		memcpy(ssbo_data, glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(SsboData), GL_MAP_READ_BIT), sizeof(SsboData));
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		/**/if (ssbo_data)
		{
			//LOG(Info) << ssbo_data->debug;

			/*glGenBuffers(1, &vbo);*/
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(SsboData), &ssbo_data->vertices[0], GL_STATIC_DRAW);
			
			// use fragment shader /vertex shader
			Shader::Use(fragmentShader.GetProgramID());
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, &_mvp[0][0]);
			glUniform3fv(color_location, 1, &testColor[0]);
		}
	}

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	glPointSize(10);
	glDrawArrays(GL_POINTS, 0, 512);/**/

	
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

void RayTracingSecond::UpdateSettingsUI()
{
	ShaderScene::UpdateSettingsUI();
	ImGui::ColorEdit3("test color", &testColor[0]);

	static string _temp;
	for (int i = 0; i < 512; i++)
	{
		_temp = "particuleID: " + std::to_string(i);
		ImGui::InputFloat3(_temp.c_str(), glm::value_ptr(ssbo_data->vertices[i]));
	}
	
}

void RayTracingSecond::Update(float _dt, glm::mat4 _mvp)
{
	Render(_dt, _mvp, GetName());
	UpdateSettingsUI();
}



void RayTracingSecond::Clean()
{
}


char* RayTracingSecond::GetName()
{
	return "Ray Tracing 2";
}