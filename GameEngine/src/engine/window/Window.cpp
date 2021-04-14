#include "Window.h"
// #include "GlDebug.hpp" todo
#include "Log.h"
#include <iostream>

/* log */
#include "GlDebug.hpp"

/* ImGUI */
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_impl_glfw.h"

using std::cout;
using std::endl;


GLFWwindow* Window::window=nullptr;
glm::vec2  Window::size=glm::vec2(0);

Window::Window(const std::string& _title) : title(_title), previousSeconds(0), currentSeconds(0), frameCount(0)
{}

Window::~Window()
{
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
    size = glm::vec2(_width, _height);
    if (!InitGLFW()) return false;
    
    window = glfwCreateWindow(_width, _height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        LOG(Info) << "WindowGLFW Failed !";
        return false;
    }
    LOG(Info) << "WindowGLFW initialized";

    
    glfwMakeContextCurrent(window);

	
    if(!InitGLEW()) return false;
	
    if (!InitImGUI()) return false;
	
    // Size of the viewport 
    //glViewport(0, 0, _width, _height);

    // Enable transparency
    glEnable(GL_BLEND);


    // OpenGL error callback
    if (glDebugMessageControlARB != nullptr)
    {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback((GLDEBUGPROCARB)debugGlErrorCallback, nullptr);
        GLuint unusedIds = 0;
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, GL_TRUE);
    }
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    // Enable depth test
    //glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    //glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
    glfwSetCursorPos(window, _width / 2, _height / 2);
    // Window color

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

bool Window::WindowShouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::UpdateInputEvent()
{
    glfwPollEvents();
}

void Window::UpdateBackgroundColor(float _r, float _g, float _b, float _a)
{
    glClearColor(_r, _g, _b, _a);
}

void Window::NewImGUIFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Window::ImGUIRender()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Window::CloseWindow()
{
    return glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0;
}

GLFWwindow* Window::GetWindow() const
{
    return window;
}

void Window::GetCursorPosition(double& _x, double& _y)
{
    glfwGetCursorPos(window, &_x, &_y);
}

bool Window::InitGLFW()
{
    if (!glfwInit())
    {
        const char** _error = nullptr;
        glfwGetError(_error);
        LOG(Error) << "GLFW initialisation failed";
        LOG(Error) << _error;
        return false;
    }
    LOG(Info) << "GLFW Initialized";

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
   // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	return true;
}

bool Window::InitGLEW()
{
    if (!window) return false;
    // OpenGL setup
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        LOG(Info) << "GLEW Failed !";
        return false;
    }
    LOG(Info) << "GLEW initialized";
    LOG(Info) << "OPENGL Version : " << glGetString(GL_VERSION);

    return true;
}

bool Window::InitImGUI()
{
    if (!window) return false;
    // setup IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

    return true;
}
