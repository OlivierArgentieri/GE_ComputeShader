#include "Shader.h"

Shader::Shader() {}

Shader::~Shader() {}

void Shader::CompileVertexShader()
{
    // === Vertex and Fragment shaders ===
    std::string _fileVertexShader;
    std::ifstream _vertexShaderFile(
	    R"(D:\Projet\PullGithub\IntroductionToShadersAndOpenGL_Artfx\IntroductionToShadersAndOpenGL\5_Refactoring\build\assets\test.vert)");
    if (_vertexShaderFile.is_open())
    {
        std::string _line;
        while (std::getline(_vertexShaderFile, _line))
        {
            _fileVertexShader += (_line.c_str());
            _fileVertexShader += "\n";
        }
    }
    vertexShader = _fileVertexShader.c_str();
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, NULL);
    glCompileShader(vs);
    CheckShaderErrors(vs, "vertex");
}

void Shader::CompileFragmentShader()
{
    std::string _filefragmentShader;
    std::ifstream _fragmentShaderFile(
	    R"(D:\Projet\PullGithub\IntroductionToShadersAndOpenGL_Artfx\IntroductionToShadersAndOpenGL\5_Refactoring\build\assets\test.frag)");
    if (_fragmentShaderFile.is_open())
    {
        std::string _line;
        while (std::getline(_fragmentShaderFile, _line))
        {
            _filefragmentShader += (_line.c_str());
            _filefragmentShader += "\n";
        }
    }

    fragmentShader = _filefragmentShader.c_str();
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, NULL);
    glCompileShader(fs);
    CheckShaderErrors(fs, "fragment");
}

void Shader::CreateShaderProgram()
{
    // create program
    programId = glCreateProgram();
    glAttachShader(programId, fs);
    glAttachShader(programId, vs);
    glLinkProgram(programId);


    // debug shader
    int _params = -1;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &_params);
    if (!_params)
    {
        LOG(Error) << "Could not link shader programme GL index " << programId;
        PrintShaderInfoLog(vs);
    }

    if (!IsValid(programId))
    {
        LOG(Error) << "Could not validate shader" << programId;
    }

    // delete shader
    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader& Shader::Use()
{
    glUseProgram(programId);
    return *this;
}

void Shader::CheckShaderErrors(GLuint _shader, std::string _shaderType)
{
    // debug shader
    int _params = -1;
    glGetShaderiv(_shader, GL_COMPILE_STATUS, &_params);
    if (!_params)
    {
        LOG(Error) << "GL " << _shaderType << " shader index " << _shader << " did not compile.";
        PrintShaderInfoLog(vs);
    }
}

void Shader::PrintShaderInfoLog(GLuint _shaderIndex)
{
    int _maxLength = 2048;
    int _actualLength = 0;
    char _log[2048];

    glGetShaderInfoLog(_shaderIndex, _maxLength, &_actualLength, _log);
    LOG(Info) << "Shader info log for GL index" << _shaderIndex;
    LOG(Info) << _log;
}

void Shader::PrintProgramInfoLog(GLuint _programid)
{
    int _maxLength = 2048;
    int _actualLength = 0;
    char _log[2048];

    glGetProgramInfoLog(_programid, _maxLength, &_actualLength, _log);
    LOG(Info) << "Shader info log for GL index" << _programid;
    LOG(Info) << _log;
}

const char* Shader::GLTypeToString(GLenum _type)
{
    switch (_type)
    {
    case GL_BOOL: return "bool";
    case GL_INT: return "int";
    case GL_FLOAT: return "float";
    case GL_FLOAT_VEC2: return "vec2";
    case GL_FLOAT_VEC3: return "vec3";
    case GL_FLOAT_VEC4: return "vec4";
    case GL_FLOAT_MAT2: return "mat2";
    case GL_FLOAT_MAT3: return "mat3";
    case GL_FLOAT_MAT4: return "mat4";
    case GL_SAMPLER_2D: return "sampler2D";
    case GL_SAMPLER_3D: return "sampler3D";
    case GL_SAMPLER_CUBE: return "samplerCube";
    case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";

    default: return "other";

    }
}

void Shader::PrintAllParams(GLuint _programID)
{
    LOG(Info) << "===========================";
    LOG(Info) << "Shader program " << programId << " info";
    int _params = -1;
    glGetProgramiv(_programID, GL_LINK_STATUS, &_params);
    LOG(Info) << "GL_LINK_STATUS = " << _params;

    glGetProgramiv(_programID, GL_ATTACHED_SHADERS, &_params);
    LOG(Info) << "GL_ATTACHED_SHADERS = " << _params;

    glGetProgramiv(_programID, GL_ACTIVE_ATTRIBUTES, &_params);
    LOG(Info) << "GL_ACTIVE_ATTRIBUTES = " << _params;

    for (GLuint _i = 0; _i < (GLuint)_params; _i++)
    {
        char _name[64];
        int _maxLength = 64;
        int _actualLength = 0;
        int _size = 0;
        GLenum _type;
        glGetActiveAttrib(_programID, _i, _maxLength, &_actualLength, &_size, &_type, _name);
        if (_size > 1)
        {
            for (int _j = 0; _j < _size; _j++)
            {
                char _longName[77];
#if __linux__
                sprintf(_longName, "%s[%_i]", _name, _j);
#else
                sprintf_s(_longName, "%s[%_i]", _name, _j);
#endif
                int _location = glGetAttribLocation(_programID, _longName);
                LOG(Info) << " " << _i << ") type:" << GLTypeToString(_type) << " name:" << _longName << " location:" << _location;
            }
        }
        else
        {
            int _location = glGetAttribLocation(_programID, _name);
            LOG(Info) << " " << _i << ") type:" << GLTypeToString(_type) << " name" << _name << " location" << _location;
        }
    }

    glGetProgramiv(_programID, GL_ACTIVE_UNIFORMS, &_params);
    LOG(Info) << "GL_ACTIVE_UNIFORMS = " << _params;
    for (GLuint _i = 0; _i < (GLuint)_params; _i++)
    {
        char _name[64];
        int _maxLength = 64;
        int _actualLength = 0;
        int _size = 0;
        GLenum _type;
        glGetActiveUniform(_programID, _i, _maxLength, &_actualLength, &_size, &_type, _name);
        if (_size > 1)
        {
            for (int _j = 0; _j < _size; _j++)
            {
                char _longName[77];
                sprintf(_longName, "%s[%_i]", _name, _j);
                int _location = glGetUniformLocation(_programID, _longName);
                LOG(Info) << " " << _i << ") type: " << GLTypeToString(_type) << "name: " << _longName << " location:" << _location;
            }
        }
        else
        {
            int _location = glGetUniformLocation(_programID, _name);
            LOG(Info) << " " << _i << ") type:" << GLTypeToString(_type) << " name:" << _name << " location:" << _location;
        }
    }
    PrintProgramInfoLog(_programID);
}

bool Shader::IsValid(GLuint _programID)
{
    glValidateProgram(_programID);
    int _params = -1;
    glGetProgramiv(_programID, GL_VALIDATE_STATUS, &_params);
    LOG(Info) << "program " << _programID << " GL_VALIDATE_STATUS = " << _params;
    if (_params != GL_TRUE)
    {
        PrintProgramInfoLog(_programID);
        return false;
    }
    return true;
}
