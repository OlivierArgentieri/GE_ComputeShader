#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "imgui.h"
#include "Texture.h"
#include "../engine/loaders/obj/obj_loader.h"

Game::Game() : isRunning(false), windowWidth(0), windowHeight(0)
{
}

Game::~Game()
{}

void Game::Init(int _screenWidth, int _screenHeight)
{
    windowWidth = _screenWidth;
    windowHeight = _screenHeight;
    isRunning = true;

    RegisterShaderScene(&sampleScene);
	RegisterShaderScene(&objScene);
}

void Game::Load()
{
	
    for (auto& shader_scene : shaderScenes)
    {
	    shader_scene->Init();
    }	
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
    computeMVP();
	for (auto&& shader_scene : shaderScenes)
	{
        ImGui::Begin(shader_scene->GetName().c_str());
        {
            string _childName = shader_scene->GetName() + "_render";
            ImGui::BeginChild(_childName.c_str());
            {
				shader_scene->Update(_dt, mvp);
            } ImGui::EndChild();

        }ImGui::End();
	}
	
    //int matrixLocation = glGetUniformLocation(shader.GetProgramID(), "matrix");
    //glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);
    
}

void Game::Render()
{
    //glBindVertexArray(vao);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Game::Clean()
{
    for (auto&& shader_scene : shaderScenes)
    {
        shader_scene->Clean();
    }
}

void Game::RegisterShaderScene(ShaderScene* _shaderScene)
{
    if (!_shaderScene) return;
    shaderScenes.push_back(_shaderScene);
}

void Game::UnregisterShaderScene(ShaderScene* _shaderScene)
{
    if (!_shaderScene) return;

    shaderScenes.erase(std::remove_if(shaderScenes.begin(), shaderScenes.end(), [&](auto const& _shader) {return _shader == _shaderScene; }));
}

void Game::computeMVP()
{
    const float fov = 45.0f;
    const float horizontalAngle = 3.14f;
    const float verticalAngle = 0.0f;
	
    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );
    glm::vec3 up = glm::cross(right, direction);
	
    glm::vec3 position = glm::vec3(0, 0, 5);
    projectionMatrix = glm::perspective(glm::radians(fov), 4.0f / 3.0f, 0.1f, 100.0f);
    viewMatrix = glm::lookAt(
        position,           // Camera is here
        position + direction, // and looks here : at the same position, plus "direction"
        up                  // Head is up (set to 0,-1,0 to look upside-down)
    );
    mvp = projectionMatrix * viewMatrix * glm::mat4(1.0);
}
