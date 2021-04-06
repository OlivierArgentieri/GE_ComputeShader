#pragma once

#include "Log.h"
#include <GL/glew.h>

class Shader
{
public:
    Shader();
    virtual ~Shader();

    GLuint programId;

    void CompileVertexShader();
    void CompileFragmentShader();
    void CreateShaderProgram();

    // set current shader
    Shader& Use();

private:
    GLuint vs;
    GLuint fs;

    const char* vertexShader;
    const char* fragmentShader;

    void CheckShaderErrors(GLuint _shader, std::string _shaderType);

    void PrintShaderInfoLog(GLuint _shaderIndex);
    void PrintProgramInfoLog(GLuint _programId);
    const char* GLTypeToString(GLenum _type);
    void PrintAllParams(GLuint _programId);
    bool IsValid(GLuint _programId);

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
};
