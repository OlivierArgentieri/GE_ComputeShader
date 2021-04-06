#include "Window.h"
// #include "GlDebug.hpp" todo
#include "Log.h"

#include <iostream>

#include "GlDebug.hpp"

using std::cout;
using std::endl;

Window::Window(const std::string& _title) : title(_title), previousSeconds(0), currentSeconds(0), frameCount(0)
{}

Window::~Window()
{
    glfwDestroyWindow(window);
    LOG(Info) << "Bye";
}

void Window::UpdateFpsCounter(float _dt)
{
    double _elapsedSeconds;

    currentSeconds += _dt;
    _elapsedSeconds = currentSeconds - previousSeconds;
    /* limit text updates to 4 per seconds */

    if (_elapsedSeconds > 0.25)
    {
        previousSeconds = currentSeconds;
        char _tmp[128];
        double _fps = (double)frameCount / _elapsedSeconds;
        sprintf_s(_tmp, "%s @ fps: %.2f", title.c_str(), _fps);
        glfwSetWindowTitle(window, _tmp);
        frameCount = 0;
    }
    frameCount++;
}

bool Window::Init(int _width, int _height)
{
    if (!glfwInit())
    {
	    const char** _error=nullptr;
        glfwGetError(_error);
        LOG(Error) << "GLFW initialisation failed";
        LOG(Error) << _error;
        return false;
    }
    LOG(Info) << "GLFW Initialized";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // openGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	
    window = glfwCreateWindow(_width, _height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        LOG(Info) << "WindowGLFW Failed !";
        return false;
    }
    LOG(Info) << "WindowGLFW initialized";

    
    glfwMakeContextCurrent(window);
    // OpenGL setup
    if (glewInit() != GLEW_OK)
    {
        LOG(Info) << "GLEW Failed !";
        return false;
    }
    LOG(Info) << "GLEW initialized";

    // Size of the viewport 
    glViewport(0, 0, _width, _height);

    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // OpenGL error callback
    if (glDebugMessageControlARB != nullptr)
    {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback((GLDEBUGPROCARB)debugGlErrorCallback, nullptr);
        GLuint unusedIds = 0;
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, GL_TRUE);
    }

    // Enable cull face optimization
    glEnable(GL_CULL_FACE); // cull face
    glCullFace(GL_BACK); // cull back face
    glFrontFace(GL_CCW); // GL_CCW for Counter Clock-Wise


    // Window color
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    return true;
}

void Window::LogGlParams()
{
    GLenum _params[] =
    {
        GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
        GL_MAX_CUBE_MAP_TEXTURE_SIZE,
        GL_MAX_DRAW_BUFFERS,
        GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
        GL_MAX_TEXTURE_IMAGE_UNITS,
        GL_MAX_TEXTURE_SIZE,
        GL_MAX_VARYING_FLOATS,
        GL_MAX_VERTEX_ATTRIBS,
        GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
        GL_MAX_VERTEX_UNIFORM_COMPONENTS,
        GL_MAX_VIEWPORT_DIMS,
        GL_STEREO,
    };

    const char* _names[] =
    {
        "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
        "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
        "GL_MAX_DRAW_BUFFERS",
        "GL_FRAGMENT_UNIFORM_COMPONENTS",
        "GL_MAX_TEXTURE_UNITS",
        "GL_MAX_TEXTURE_SIZE",
        "GL_MAX_VARYING_FLOATS",
        "GL_MAX_VERTEX_ATTRIBS",
        "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_VERTEX_TEXTURE_UNIFORM_COMPONENTS",
        "GL_MAX_VIEWPORT_DIMS",
        "GL_STEREO"
    };

    LOG(Info) << "===============";
    LOG(Info) << "GL Context Params: ";
    // interger only in orderi 0-10 return types //

    for (int _i = 0; _i < 10; _i++)
    {
        int _v = 0;
        glGetIntegerv(_params[_i], &_v);
        LOG(Info) << _names[_i] << " " << _v;
    }
    // others
    int _v[2];
    _v[0] = _v[1] = 0;
    glGetIntegerv(_params[10], _v);
    LOG(Info) << _names[10] << " " << _v[0] << " " << _v[1];
    unsigned char _s = 0;

    glGetBooleanv(_params[11], &_s);
    LOG(Info) << _names[11] << " " << (unsigned int)_s;
    LOG(Info) << "=============";
}

void Window::ClearBuffer()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen
}

void Window::SwapBuffer()
{
    glfwSwapBuffers(window);
}

void Window::Clean()
{
    glfwDestroyWindow(window);
}