#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_impl_glfw.h"

// custom glfw window

#include <iostream>
#include <Windows.h>


#include "Game.h"
#include "Log.h"
#include "Timer.h"
#include "Window.h"
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

void compileShader(GLuint shaderID, char* sourcePointer) {
	glShaderSource(shaderID, 1, &sourcePointer, NULL);
	glCompileShader(shaderID);

	GLint Result = GL_FALSE;
	int InfoLogLength = 1024;
	char shaderErrorMessage[1024] = { 0 };

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);

	glGetShaderInfoLog(shaderID, InfoLogLength, NULL, shaderErrorMessage);
	if (strlen(shaderErrorMessage) != 0)
		std::cout << shaderErrorMessage << "\n";
}

void loadShader(char** shaderBuffer, const char* fileName) {
	FILE* shader = fopen(fileName, "r");
	(*shaderBuffer) = new char[1280];
	for (int i = 0; i < 1280; i++) {
		(*shaderBuffer)[i] = (unsigned char)fgetc(shader);
		if ((*shaderBuffer)[i] == EOF) {
			(*shaderBuffer)[i] = '\0';
			break;
		}
	}
	fclose(shader);
}

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

	// setup IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(_window.GetWindow(), true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
	
	// InitGame
	 // Init Game
	Timer _timer;
	Game _game;
	_game.Init(SCREEN_WIDTH, SCREEN_WIDTH);
	_game.Load();

	float _dt = 0;
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

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

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
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//_game.Render();
		_window.SwapBuffer();
		_timer.DelayTime();
		_window.UpdateInputEvent();
	}

	_game.Clean();
	_window.Clean();

	return 0;
	
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

	/* Cr�ation de la texture #1# */
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

	GLuint computeShaderID;
	GLuint csProgramID;
	char* computeShader = 0;

	GLint Result = GL_FALSE;
	int InfoLogLength = 1024;
	char ProgramErrorMessage[1024] = { 0 };

	computeShaderID = glCreateShader(GL_COMPUTE_SHADER);

	//loadShader(&computeShader, "C:/Users/olivi/Desktop/GameEngine-master/Bin/Debug/compute.shader");
	//compileShader(computeShaderID, computeShader);

	csProgramID = glCreateProgram();

	glAttachShader(csProgramID, computeShaderID);
	glLinkProgram(csProgramID);
	glDeleteShader(computeShaderID);


	/* ----- Vertex shaders and Fragments shaders ----- #1# */

	GLuint vertexShaderID;
	GLuint fragmentShaderID;
	GLuint programID;

	Result = GL_FALSE;
	InfoLogLength = 1024;
	memset(ProgramErrorMessage, 0, 1024);

	char* vertexShader = 0;
	char* fragmentShader = 0;

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//loadShader(&vertexShader, "C:/Users/olivi/Desktop/GameEngine-master/Bin/Debug/vertex.shader");
	//loadShader(&fragmentShader, "C:/Users/olivi/Desktop/GameEngine-master/Bin/Debug/fragment.shader");

	//compileShader(vertexShaderID, vertexShader);
	//compileShader(fragmentShaderID, fragmentShader);

	programID = glCreateProgram();

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);

	if (strlen(ProgramErrorMessage) != 0)
		std::cout << ProgramErrorMessage << "\n";

	/* ----- Run Compute shader ----- #1# */
	glUseProgram(csProgramID);
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

#if defined(WIN32) || defined(_WIN32)
		Sleep(40);
#else
		usleep(40000);
#endif

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
