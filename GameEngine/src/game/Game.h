#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

#include "Shader.h"

using std::vector;

// run a simple game loop
class Game
{
public:
    Game();
    virtual ~Game();

    void Init(int _screenWidth, int _screenHeight);
    void Load();
    void HandleInputs();
    void Update(float _dt);
    void Render();
    void Clean();

    bool isRunning;
    int windowWidth, windowHeight;

private:
    Shader shader;
    GLuint vao;

    float speed = 1.0f;
    float lastPosition = 0.5f;
    float matrix[16]
    {
        1.0f, 0.0f, 0.0f, 0.0f, // first coloumn
        0.0f, 1.0f, 0.0f, 0.0f, // second
        0.0f, 0.0f, 1.0f, 0.0f, // third
        0.5f, 0.0f, 0.0f, 1.0f // fourth
    };
};