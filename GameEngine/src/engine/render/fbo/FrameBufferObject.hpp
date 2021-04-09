#pragma once
#include <glew.h>

class FrameBufferObject
{
private:
    //static FrameBufferObject* instance;
	
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