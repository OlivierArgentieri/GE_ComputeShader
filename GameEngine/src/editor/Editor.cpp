#include "Editor.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "imgui.h"
#include "RenderView.hpp"
#include "ShaderScene.hpp"
#include "../engine/loaders/obj/ObjLoader.hpp"


Editor::Editor() : isRunning(false), windowWidth(0), windowHeight(0), currentScene(0)
{
}

Editor::~Editor()
{}

void Editor::Init(int _screenWidth, int _screenHeight)
{
    windowWidth = _screenWidth;
    windowHeight = _screenHeight;
    isRunning = true;

	
    RegisterShaderScene(&sampleScene);

    RegisterShaderScene(&objScene);
}

void Editor::Load()
{
    for (auto& shader_scene : shaderScenes)
    {
        shader_scene->Init();
    }

    
}


void Editor::HandleInputs()
{
	
}


void Editor::Update(float _dt)
{
    auto&& _shader_scene = shaderScenes[currentScene];
    LOG(Info) << _dt;
	computeMVP(_shader_scene->GetTransform().GetModelMatrix()); // compute with object model matrix, here is scene
    _shader_scene->Update(_dt, mvp);
}

void Editor::Render()
{
   
}

void Editor::Clean()
{
    for (auto&& shader_scene : shaderScenes)
    {
        shader_scene->Clean();
    }
}

void Editor::UpdateUI()
{
	// create menu
    bool _useless = true;
    ImGui::Begin("Scenes", &_useless, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);
    ImGui::SetWindowPos(ImVec2(0, 18));
    ImGui::SetWindowSize(ImVec2(windowWidth*0.2,windowHeight*0.2));

    ImGui::TextColored(ImVec4(0.45f, 0.55f, 0.60f, 1.00f), "Scenes");

    ImGui::ListBox(" ", &currentScene, shaderScenesNames.data(), shaderScenes.size(), 4);
    ImGui::End();
}

void Editor::RegisterShaderScene(ShaderScene* _shaderScene)
{
    if (!_shaderScene) return;
    shaderScenes.push_back(_shaderScene);
    const char* _test = _shaderScene->GetName();
    shaderScenesNames.push_back(_shaderScene->GetName());
}

void Editor::UnregisterShaderScene(ShaderScene* _shaderScene)
{
    if (!_shaderScene) return;

    shaderScenes.erase(std::remove_if(shaderScenes.begin(), shaderScenes.end(), [&](ShaderScene* _shader) {return _shader->GetName() == _shaderScene->GetName(); }));
    shaderScenesNames.erase(std::remove_if(shaderScenesNames.begin(), shaderScenesNames.end(), [&](const char* _name) {return _name == _shaderScene->GetName(); }));
}

void Editor::computeMVP(glm::mat4 _objectModelMatrix = glm::mat4(1.0))
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
