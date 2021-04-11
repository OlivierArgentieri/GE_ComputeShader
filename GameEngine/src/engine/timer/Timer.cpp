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

Timer::Timer()
{}

Timer::~Timer()
{}
