#pragma once

#include <GL/glew.h>
#include <map>

#include "Log.h"

// Breakpoints that should ALWAYS trigger (even in release)
#ifdef _MSC_VER
#define eTB_CriticalBreakPoint() \
    if (IsDebuggerPresent()) __debugbreak();
#else
#define eTB_CriticalBreakPoint() asm(" int $3");
#endif

const char* debugGlSourceToStr(GLenum _source)
{
    static std::map<int, const char*> _sources =
    {
        {GL_DEBUG_SOURCE_API, "API"},
        {GL_DEBUG_SOURCE_WINDOW_SYSTEM, "Window System"},
        {GL_DEBUG_SOURCE_SHADER_COMPILER, "Shader Compiler"},
        {GL_DEBUG_SOURCE_THIRD_PARTY, "Third Party"},
        {GL_DEBUG_SOURCE_APPLICATION, "Application User"},
        {GL_DEBUG_SOURCE_OTHER, "Other"},
    };
    return _sources[_source];
}

const char* debugGlTypeToStr(GLenum _type)
{
    static std::map<int, const char*> _types =
    {
        {GL_DEBUG_TYPE_ERROR, "Error"},
        {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "Deprecated Behaviour"},
        {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "Undefined Behaviour"},
        {GL_DEBUG_TYPE_PORTABILITY, "Portability"},
        {GL_DEBUG_TYPE_PERFORMANCE, "Performance"},
        {GL_DEBUG_TYPE_MARKER, "Marker"},
        {GL_DEBUG_TYPE_PUSH_GROUP, "Debug Push Group"},
        {GL_DEBUG_TYPE_POP_GROUP, "Debug Pop Group"},
        {GL_DEBUG_TYPE_OTHER, "Other"},
    };
    return _types[_type];
}

const char* debugGlSeverityStr(GLenum _severity)
{
    static std::map<int, const char*> _severities =
    {
        {GL_DEBUG_SEVERITY_MEDIUM, "Medium"},
        {GL_DEBUG_SEVERITY_LOW, "Low"},
        {GL_DEBUG_SEVERITY_NOTIFICATION, "Notification"},
    };
    return _severities[_severity];
}

void debugGlErrorCallback(GLenum _source, GLenum _type, GLuint _id, GLenum _severity, GLsizei _length, const GLchar* _message, GLvoid* _userParam)
{
    std::string glLog("= OpenGL ============");
    glLog.append("\n            "); glLog.append("message: "); glLog.append(_message);
    glLog.append("\n            "); glLog.append("type: "); glLog.append(debugGlTypeToStr(_type));
    glLog.append("\n            "); glLog.append("id: "); glLog.append(std::to_string(_id));
    glLog.append("\n            "); glLog.append("severity: "); glLog.append(debugGlSeverityStr(_severity));

    if (_type == GL_DEBUG_TYPE_ERROR)
    {
        LOG(Error) << glLog;
    }
    else if (_type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR || _type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR || _type == GL_DEBUG_TYPE_PORTABILITY || _type == GL_DEBUG_TYPE_PERFORMANCE)
    {
        LOG(Warning) << glLog;
    }
    else
    {
        LOG(Debug) << glLog;
    }
}
