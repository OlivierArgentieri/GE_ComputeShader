#include "ImGUI/imgui.h"

#include "Game.h"
#include "Log.h"
#include "Timer.h"
#include "Window.h"

LogConfig LOG_CONFIG = {};


int main(void)
{
	const int SCREEN_WIDTH = 1024;
	const int SCREEN_HEIGHT = 768;

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

	

	
	/* ----- Render loop ----- #1# */
	float _dt;
	while (_game.isRunning)
	{
		_dt = static_cast<float>(_timer.ComputeDeltaTime());
		_window.UpdateFpsCounter(_dt);
		_window.UpdateBackgroundColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

		_game.HandleInputs();
		

		_window.ClearBuffer();
		_window.NewImGUIFrame();

		ImGui::BeginMainMenuBar();
		ImGui::Text("shader tests");
		ImGui::EndMainMenuBar();

		/* render scene here */
		_game.Update(_dt);
		
		_window.ImGUIRender();
		_game.Render();
		_window.SwapBuffer();
		_window.UpdateInputEvent();
		_game.isRunning = _window.CloseWindow();
		_timer.DelayTime();
	}
	
	return 0;
}
