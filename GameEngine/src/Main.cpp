#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ImGUI/imgui.h"

// custom glfw window

#include <iostream>
#include <Windows.h>


#include "Game.h"
#include "Log.h"
#include "Timer.h"
#include "Window.h"
#include "engine/loaders/obj/obj_loader.h"
LogConfig LOG_CONFIG = {};

typedef struct _COORDS_ {
	GLfloat x;
	GLfloat y;
	GLfloat s;
	GLfloat t;
} CoordinatesSet;

typedef struct _RGBA_ {
	GLfloat Red;
	GLfloat Green;
	GLfloat Blue;
	GLfloat Alpha;
} RGBAValues;

int main(void)
{
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 640;

	// Init Logging
	// Log init
	LOG_CONFIG.reporting_level = Debug;
	LOG_CONFIG.restart = true;
	if (LOG_CONFIG.restart) { Log::restart(); }

	
	// Init Window
	/**/Window _window = Window("Hello framework");
	if (!_window.Init(SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		return 1;
	}

	
	
	// Init Game
	Timer _timer;
	Game _game;
	_game.Init(SCREEN_WIDTH, SCREEN_WIDTH);
	_game.Load();


	// test load obj
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; // Won't be used at the moment.
	bool res = loadOBJ("D:/Projet/PullGithub/GE_CustomShader/Bin/Debug/cube.obj", vertices, uvs, normals);
	
	/*float _dt = 0;
	bool show_demo_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	while (_game.isRunning && !_window.WindowShouldClose())
	{
		_dt = static_cast<float>(_timer.ComputeDeltaTime());
		_window.UpdateFpsCounter(_dt);
		_window.UpdateBackgroundColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		
		//_game.HandleInputs();
		//_game.Update(_dt);

		_window.ClearBuffer();
		_window.NewImGUIFrame();

		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glEnd();
		

		// one window
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		_window.ImGUIRender();
		//_game.Render();
		_window.SwapBuffer();
		_timer.DelayTime();
		_window.UpdateInputEvent();
	}*/

	
	
	/* ----- Quad Context ----- #1#*/
	RGBAValues* quadTexture = new RGBAValues[640 * 480];
	CoordinatesSet* quadVertex = new CoordinatesSet[4];
	unsigned short int* quadIndex = new unsigned short int[3];

	quadVertex[0].x = -1.0f;
	quadVertex[0].y = 1.0f;

	quadVertex[1].x = -1.0f;
	quadVertex[1].y = -1.0f;

	quadVertex[2].x = 1.0f;
	quadVertex[2].y = 1.0f;

	quadVertex[3].x = 1.0f;
	quadVertex[3].y = -1.0f;

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

	for (int x = 0; x < 640; x++) {
		for (int y = 0; y < 480; y++) {
			quadTexture[x + y * 640].Red = 1.0f;
			quadTexture[x + y * 640].Green = .5f;
			quadTexture[x + y * 640].Blue = .0f;
			quadTexture[x + y * 640].Alpha = 1.0f;
		}
	}


	/* ----- Render Context ----- #1#*/

	GLuint quadIBO;
	GLuint quadVBO;
	GLuint quadVAO;
	GLuint quadTextureID;

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

	glGenBuffers(1, &quadIBO);
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
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	/* ----- Compute Shader ----- #1# */
	Shader _computeShader;
	char* computeShader = 0;


	/**/
	_computeShader.LoadShader("D:/Projet/PullGithub/GE_CustomShader/Bin/Debug/compute.shader", GL_COMPUTE_SHADER);
	_computeShader.CompileShader(computeShader);
	_computeShader.CreateShaderProgram();
	
	/* ----- Vertex shaders and Fragments shaders ----- #1# */
	Shader _vertexShader;
	Shader _fragmentShader;
	GLuint programID;


	char* vertexShader = 0;
	char* fragmentShader = 0;
	
	_vertexShader.LoadShader("D:/Projet/PullGithub/GE_CustomShader/Bin/Debug/vertex.shader", GL_VERTEX_SHADER);
	_fragmentShader.LoadShader("D:/Projet/PullGithub/GE_CustomShader/Bin/Debug/fragment.shader", GL_FRAGMENT_SHADER);

	_vertexShader.CompileShader(vertexShader);
	_fragmentShader.CompileShader(fragmentShader);

	_vertexShader.CreateShaderProgram();
	programID = _vertexShader.GetProgramID();
	_fragmentShader.CreateShaderProgram(programID);/**/
	

	/* ----- Run Compute shader ----- #1# */
	_computeShader.Use();
	glBindTexture(GL_TEXTURE_2D, quadTextureID);
	glBindImageTexture(0, quadTextureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute(40, 30, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);


	/* ----- Render loop ----- #1# */
	while (true) {

		glfwPollEvents();

		Sleep(40);

		glEnable(GL_CULL_FACE);

		/* ----- Actual render ----- #1# */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		glBindVertexArray(quadVAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, quadTextureID);

		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (GLvoid*)0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(0);

		glBindVertexArray(0);

		glUseProgram(0);

		_window.SwapBuffer();
	}

	return 0;

}
