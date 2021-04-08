#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Log.h"
#include <string>


class Window
{
public:
    Window(const std::string& title);
    ~Window();

    void UpdateFpsCounter(float _dt);

    bool Init(int _width, int _height);
    void LogGlParams();
    void ClearBuffer();
    void SwapBuffer();
    void Clean();
    bool WindowShouldClose();
	
    void UpdateInputEvent();
    void UpdateBackgroundColor(float _r, float _g, float _b, float _a);

    void NewImGUIFrame();
    void ImGUIRender();
    bool CloseWindow();

	GLFWwindow* GetWindow() const;
    GLFWwindow* window;

private:
    bool InitGLFW();
    bool InitGLEW();
    bool InitImGUI();
	
    const std::string& title;

    // Delete
    Window() = delete;
    //Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    double previousSeconds;
    double currentSeconds;
    int frameCount;

};