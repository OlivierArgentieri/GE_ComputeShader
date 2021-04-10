#include "FrameBufferObject.hpp"

//FrameBufferObject* FrameBufferObject::instance = nullptr;
const unsigned int FrameBufferObject::SIZE_X_VIEWPORT = 1024;
const unsigned int FrameBufferObject::SIZE_Y_VIEWPORT = 960;

FrameBufferObject::FrameBufferObject()
{
    /* create to texture to render */
    glGenTextures(1, &textureIdFBO);
    glBindTexture(GL_TEXTURE_2D, textureIdFBO);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SIZE_X_VIEWPORT, SIZE_Y_VIEWPORT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    /* create FBO */
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureIdFBO, 0);
}

GLuint FrameBufferObject::CreateRenderTexture()
{
	/* create draw buffer */
    glDrawBuffers(1, DrawBuffers);

    // render to texture
    glBindTexture(GL_TEXTURE_2D, textureIdFBO);
    glActiveTexture(GL_TEXTURE0);

	// start bind to fbo
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, SIZE_X_VIEWPORT, SIZE_Y_VIEWPORT);


	// NEED TO CLEAR 
    glClearColor(0.1f, 0.10f, 0.10f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return textureIdFBO;
}

void FrameBufferObject::EndFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
