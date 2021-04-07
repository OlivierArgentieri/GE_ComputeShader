#include "Game.h"



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
	for (auto&& shader_scene : shaderScenes)
	{
        ImGui::Begin(shader_scene->GetName().c_str());
        {
            string _childName = shader_scene->GetName() + "_render";
            ImGui::BeginChild(_childName.c_str());
            {
				shader_scene->Update();
            } ImGui::EndChild();

        }ImGui::End();
	}
	
    int matrixLocation = glGetUniformLocation(shader.GetProgramID(), "matrix");
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);
}

void Game::Render()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
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