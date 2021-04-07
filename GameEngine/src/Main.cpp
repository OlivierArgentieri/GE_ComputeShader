#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ImGUI/imgui.h"

// custom glfw window

#include <iostream>
#include <Windows.h>


#include "Game.h"
#include "Log.h"
#include "SampleScene.hpp"
#include "ShaderScene.hpp"
#include "Timer.h"
#include "Window.h"

#include "engine/loaders/obj/obj_loader.h"
LogConfig LOG_CONFIG = {};


int main(void)
{
	const int SCREEN_WIDTH = 1600;
	const int SCREEN_HEIGHT = 1280;

	// Init Logging
	// Log init
	LOG_CONFIG.reporting_level = Debug;
	LOG_CONFIG.restart = true;
	if (LOG_CONFIG.restart) { Log::restart(); }
	
	// Init Window
	/**/Window _window = Window("openGL window");
	if (!_window.Init(SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		return 1;
	}

	// Init Game
	Timer _timer;
	Game _game;
	_game.Init(SCREEN_WIDTH, SCREEN_WIDTH);
	_game.Load();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



	/* === load shader scene objects ===*/
	SampleScene _sampleScene("SampleScene");

	ShaderScene* _currentScene = &_sampleScene;

	
	/* ----- Render loop ----- #1# */
	_currentScene->Init();
	float _dt;
	while (_game.isRunning && !_window.WindowShouldClose())
	{
		_window.UpdateInputEvent();
		
		_dt = static_cast<float>(_timer.ComputeDeltaTime());
		_window.UpdateFpsCounter(_dt);
		_window.UpdateBackgroundColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

		//_game.HandleInputs();
		//_game.Update(_dt);

		_window.ClearBuffer();
		_window.NewImGUIFrame();

		// one window
		ImGui::BeginMainMenuBar();

		/* render scene here */
		_currentScene->Update();

		
		ImGui::Text("shader tests");
		ImGui::EndMainMenuBar();
		
		
	
		
		_window.ImGUIRender();
		//_game.Render();
		_window.SwapBuffer();
		_timer.DelayTime();
	}
	_currentScene->Clear();
	return 0;
}
