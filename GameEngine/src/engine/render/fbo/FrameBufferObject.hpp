#pragma once
#include <glew.h>

class FrameBufferObject
{
private:
    GLuint textureIdFBO;
    GLuint fbo;
    GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0 };
	
public:
    FrameBufferObject() = default;
    GLuint CreateRenderTexture();
    void EndFBO();

	// remove unused constructor
	FrameBufferObject(const FrameBufferObject&) = delete;
};