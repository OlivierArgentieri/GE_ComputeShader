#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Log.h"
#include <string>
#include <ext/vector_float2.hpp>


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

    static void GetCursorPosition(double& _x, double& _y);
    static glm::vec2 GetSize() { return size; }

private:
	
    bool InitGLFW();
    bool InitGLEW();
    bool InitImGUI();
	
    const std::string& title;
    static GLFWwindow* window;
    static glm::vec2 size;
    // Delete
    Window() = delete;
    //Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    double previousSeconds;
    double currentSeconds;
    int frameCount;

};
