#include "Game.h"
#include "Texture.h"

Game::Game() : isRunning(false), windowWidth(0), windowHeight(0)
{}

Game::~Game()
{}

void Game::Init(int _screenWidth, int _screenHeight)
{
    windowWidth = _screenWidth;
    windowHeight = _screenHeight;
    isRunning = true;
}


void Game::Load()
{
	
}


void Game::HandleInputs()
{
	/*
    SDL_Event _event;
    while (SDL_PollEvent(&_event))
    {
        switch (_event.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_KEYDOWN:
            if (_event.key.keysym.sym == SDLK_ESCAPE)
            {
                isRunning = false;
            }
            break;

        default:
            break;
        }
    }*/
}


void Game::Update(float _dt)
{
    /* if()
         speed =-speed;
     */

    if (lastPosition > 1.0f)
    {
        speed = -speed;
    }
    else if (lastPosition < -1.0)
    {
        speed = -speed;
    }
    matrix[12] = speed * _dt + lastPosition;
    lastPosition = matrix[12];
    shader.Use();
    int matrixLocation = glGetUniformLocation(shader.programId, "matrix");
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);
}

void Game::Render()
{
    //shader.Use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Game::Clean()
{}