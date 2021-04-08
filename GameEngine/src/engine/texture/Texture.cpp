#include "Texture.h"
#include <GL/glew.h>
#include <sstream>
#include "Log.h"


Texture::Texture() : textureID(0), filename(), width(0), height(0)
{}

Texture::~Texture() {}

void Texture::Unload()
{
    glDeleteTextures(1, &textureID);
}


void Texture::UpdateInfo(int& _widthOut, int& _heightOut)
{
    _widthOut = width;
    _heightOut = height;
}

void Texture::Use() const
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}