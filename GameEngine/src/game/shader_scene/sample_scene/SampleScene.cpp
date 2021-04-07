#include "SampleScene.hpp"

#include "imgui.h"

// from https://zestedesavoir.com/tutoriels/1554/introduction-aux-compute-shaders/
// to test this OpenGL environments 

SampleScene::SampleScene()
{
	name = "Sample shader Scene";
}

void SampleScene::Init()
{
	/* ----- Quad Context ----- #1#*/
	CoordinatesSet* quadVertex = new CoordinatesSet[4];
	unsigned short int* quadIndex = new unsigned short int[3];

	quadVertex[0].x = -0.5f;
	quadVertex[0].y = 0.5f;

	quadVertex[1].x = -0.5f;
	quadVertex[1].y = -0.5f;

	quadVertex[2].x = 0.5f;
	quadVertex[2].y = 0.5f;

	quadVertex[3].x = 0.5f;
	quadVertex[3].y = -0.5f;

	quadIndex[0] = 0;
	quadIndex[1] = 1;
	quadIndex[2] = 2;
	quadIndex[3] = 3;

	quadVertex[0].s = 0.0f;
	quadVertex[0].t = 0.0f;
	quadVertex[1].s = 0.0f;
	quadVertex[1].t = 1.0f;
	quadVertex[2].s = 1.0f;
	quadVertex[2].t = 0.0f;
	quadVertex[3].s = 1.0f;
	quadVertex[3].t = 1.0f;

	

	/* Création de la texture #1# */
	glGenTextures(1, &quadTextureID);
	glBindTexture(GL_TEXTURE_2D, quadTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 640, 480, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	/**/glGenBuffers(1, &quadIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(unsigned short int), quadIndex, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CoordinatesSet) * 4, 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(CoordinatesSet) * 4, quadVertex);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIBO);

	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));
	glEnableVertexAttribArray(1);/**/

	glBindVertexArray(0);

	/* ----- Compute Shader ----- #1# */
	Shader _computeShader;
	char* computeShader = 0;

	_computeShader.LoadShader("assets/sample/compute.shader", GL_COMPUTE_SHADER);
	_computeShader.CompileShader(computeShader);
	_computeShader.CreateShaderProgram();

	/* ----- Vertex shaders and Fragments shaders -----  */
	Shader _vertexShader;
	Shader _fragmentShader;
	

	char* vertexShader = 0;
	char* fragmentShader = 0;

	_vertexShader.LoadShader("assets/sample/vertex.shader", GL_VERTEX_SHADER);
	_fragmentShader.LoadShader("assets/sample/fragment.shader", GL_FRAGMENT_SHADER);

	_vertexShader.CompileShader(vertexShader);
	_fragmentShader.CompileShader(fragmentShader);

	_vertexShader.CreateShaderProgram();
	programID = _vertexShader.GetProgramID();
	_fragmentShader.CreateShaderProgram(programID);/**/


	/* ----- Run Compute shader ----- #1# */
	_computeShader.Use();
	/**/glBindTexture(GL_TEXTURE_2D, quadTextureID);
	glBindImageTexture(0, quadTextureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute(40, 30, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void SampleScene::Update()
{

	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0.0f, 0.5f);
	glVertex2f(0.5f, -0.5f);
	glEnd();

	glUseProgram(programID);
	glBindVertexArray(quadVAO);

	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, quadTextureID);

	//glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (GLvoid*)0);
	ImGui::Begin(name.c_str());
	{
		string _childName = name + "_render";
		ImGui::BeginChild(_childName.c_str());
		{
			ImVec2 wsize = ImGui::GetWindowSize();
			ImGui::Image(reinterpret_cast<ImTextureID>(quadTextureID), wsize, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::EndChild();

			ImGui::Text("Shader Text");               // Display some text (you can use a format strings too)
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		ImGui::End();
	}
}

void SampleScene::Clean()
{
}
