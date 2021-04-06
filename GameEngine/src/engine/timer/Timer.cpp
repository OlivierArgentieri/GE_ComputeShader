#include "Timer.h"
#include <algorithm>
#include <GLFW/glfw3.h>

Timer::Timer() : frameStart(0), lastFrame(0), frameTime(0)
{}

Timer::~Timer()
{}

unsigned int Timer::ComputeDeltaTime()
{
    frameStart = glfwGetTime();
    unsigned int _dt = frameStart - lastFrame;
    _dt = std::min(_dt, MAX_DT);
    timeSinceStart += static_cast<float>(_dt) / 1000.0f;
    lastFrame = frameStart;
    return _dt; // clamp dt
}

void Timer::DelayTime()
{
    frameTime = glfwGetTime() - frameStart;
    if (frameTime < frameDelay)
    {
        glfwSetTime(frameDelay - frameTime);
    }
}

double Timer::timeSinceStart = 0.0f;
