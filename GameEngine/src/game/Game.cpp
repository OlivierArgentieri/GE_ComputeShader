#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "imgui.h"
#include "Texture.h"
#include "../engine/loaders/obj/ObjLoader.hpp"

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
	
}


void Game::Update(float _dt)
{
	for (auto&& shader_scene : shaderScenes)
	{
        computeMVP(shader_scene->GetTransform().GetModelMatrix()); // compute with object model matrix, here is scene
		
        ImGui::Begin(shader_scene->GetName().c_str());
        {
            string _childName = shader_scene->GetName() + "_render";
            ImGui::BeginChild(_childName.c_str());
            {
                shader_scene->Update(_dt, mvp);
            	
            } ImGui::EndChild();
        }ImGui::End();
	}

}

void Game::Render()
{
   
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

void Game::computeMVP(glm::mat4 _objectModelMatrix = glm::mat4(1.0))
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
    viewMatrix = glm::lookAt(position, position + direction,up);
    mvp = projectionMatrix * viewMatrix * _objectModelMatrix;
}
