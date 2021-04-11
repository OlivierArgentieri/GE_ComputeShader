#pragma once

class Timer
{
public:
    static float GetDeltaTime();
    static void BeginFrame();
    static void EndFrame();

	
    Timer();
    virtual ~Timer();

private:
    static double currentTime;
    static double oldTime;
    static float deltaTime;
};