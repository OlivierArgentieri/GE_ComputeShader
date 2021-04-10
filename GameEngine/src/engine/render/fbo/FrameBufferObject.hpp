#pragma once
#include <glew.h>

class FrameBufferObject
{

public:
    static const unsigned int SIZE_Y_VIEWPORT;
    static const unsigned int SIZE_X_VIEWPORT;
	
protected:
    
	
    GLuint textureIdFBO;
    GLuint fbo;
    GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0 };

public:
    FrameBufferObject();

    /*
    static FrameBufferObject* GetInstance()
    {
        if (!instance)
            instance = new FrameBufferObject();
        return instance;
    }*/
    
	
    GLuint CreateRenderTexture();
    void EndFBO();

	// remove unused constructor
	FrameBufferObject(const FrameBufferObject&) = delete;
};