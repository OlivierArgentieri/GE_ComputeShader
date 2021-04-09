#include "Timer.h"
#include <algorithm>
#include <GLFW/glfw3.h>



double Timer::currentTime = 0;;
double Timer::oldTime = 0;;
float Timer::deltaTime = 0;;

float Timer::GetDeltaTime()
{
    return deltaTime;
}

void Timer::BeginFrame()
{
    oldTime = glfwGetTime();
}

void Timer::EndFrame()
{
    currentTime = glfwGetTime();

    deltaTime = float(currentTime - oldTime);
    oldTime = currentTime;
}

Timer::Timer() : frameStart(0), lastFrame(0), frameTime(0)
{}

Timer::~Timer()
{}

unsigned int Timer::ComputeDeltaTime()
{
    frameStart = glfwGetTime();
    unsigned int _dt = frameStart - lastFrame;
    _dt = std::min(_dt, MAX_DT);
    timeSinceStart += static_cast<float>(_dt) / FPS;
    lastFrame = frameStart;
    return _dt; // clamp dt
}

void Timer::DelayTime()
{
    return;
    unsigned int _currentTime = glfwGetTime();
    frameTime = _currentTime - frameStart;
    if (FRAME_DELAY < frameTime)
    {
    	glfwSetTime(FRAME_DELAY - frameTime);
    }
}

double Timer::timeSinceStart = 0.0f;
