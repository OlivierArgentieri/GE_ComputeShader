#pragma once

class Timer
{
public:
    Timer();
    virtual ~Timer();

    // Compute delta time
    unsigned int ComputeDeltaTime();

    // wait for amount of fps
    void DelayTime();

    // Time since the game started
    static double GetTimeSinceStart() { return timeSinceStart; }

private:
    const static int FPS = 60;
    const static int FRAME_DELAY = 1000 / FPS;
    const unsigned int MAX_DT = 50;

    // Time in ms when frame starts
    unsigned int frameStart;

    // Last frame start time in ms
    unsigned int lastFrame;

    // Time it tooks the loop / framerate
    unsigned int frameTime;

    // Time (s) since the game started
    static double timeSinceStart;
};