#pragma once
#include <GL/glew.h>
#include <vector>
#include <ext/matrix_float4x4.hpp>


#include "ObjSceneTest.hpp"
#include "SampleScene.hpp"
#include "cs_to_texture/CsToTexture.hpp"
#include "obj_scene/ObjScene.hpp"

using std::vector;

// run a simple game loop
class Editor
{

private:
    int windowWidth, windowHeight;
    vector<ShaderScene*> shaderScenes;
    vector<const char*> shaderScenesNames;

public:
    Editor();
    virtual ~Editor();

    void Init(int _screenWidth, int _screenHeight);
    void Load();
    void HandleInputs();
    void Update(float _dt);
    void Render();
    void Clean();
    void UpdateUI();

    bool isRunning;
    

	/* register shader scene objects */
    void RegisterShaderScene(ShaderScene* _shaderScene);
    void UnregisterShaderScene(ShaderScene* _shaderScene);

private:
    /* === load shader scene objects ===*/
    ObjScene objScene;
    ObjSceneTest sampleScene;
    CsToTexture csToTextureScene;

	// current:
    int currentScene;
    //SampleScene sampleScene;

	/* === compute mvp === */
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    GLuint matrixID;
    glm::mat4 mvp;
    void computeMVP(glm::mat4 _objectModelMatrix);
};