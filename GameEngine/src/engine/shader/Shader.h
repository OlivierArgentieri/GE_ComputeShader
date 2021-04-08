#pragma once

#include <vector>

#include "Log.h"
#include <GL/glew.h>

class Shader
{
public:
    Shader();
    virtual ~Shader();


    void LoadShader(const char* _fileName, unsigned int _shaderType);
    void CreateShaderProgram(GLuint _programId=-1);
    void CompileShader(char* sourcePointer);
    GLuint GetProgramID() const;
    // set current shader
    Shader& Use();

private:
    GLuint sid;
    GLuint programId;

    std::string shaderBuffer;

    void CheckShaderErrors(GLuint _shader, std::string _shaderType);

    void PrintShaderInfoLog(GLuint _shaderIndex);
    void PrintProgramInfoLog(GLuint _programId);
    const char* GLTypeToString(GLenum _type);
    void PrintAllParams(GLuint _programId);
    bool IsValid(GLuint _programId);

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
};