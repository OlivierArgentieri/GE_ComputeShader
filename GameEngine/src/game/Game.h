#pragma once
#include <GL/glew.h>
#include <vector>
#include <ext/matrix_float4x4.hpp>



#include "SampleScene.hpp"
#include "Shader.h"
#include "obj_scene/ObjScene.hpp"

using std::vector;

// run a simple game loop
class Game
{

private:
    Shader shader;
    GLuint vao;

    float speed = 1.0f;
    float lastPosition = 0.5f;
    float matrix[16]
    {
        1.0f, 0.0f, 0.0f, 0.0f, // first column
        0.0f, 1.0f, 0.0f, 0.0f, // second
        0.0f, 0.0f, 1.0f, 0.0f, // third
        0.5f, 0.0f, 0.0f, 1.0f // fourth
    };

    vector<ShaderScene*> shaderScenes;


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

	/* register shader scene objects */
    void RegisterShaderScene(ShaderScene* _shaderScene);
    void UnregisterShaderScene(ShaderScene* _shaderScene);

private:
    /* === load shader scene objects ===*/
    SampleScene sampleScene;
    ObjScene objScene;

	/* === compute mvp === */
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    GLuint matrixID;
    glm::mat4 mvp;
    void computeMVP(glm::mat4 _objectModelMatrix);

	/* === FBO Object === */
   
};