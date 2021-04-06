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

bool Texture::Load(const string& _filenameP)
{
    filename = _filenameP;
    FILE* _file;
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
	
    // Load from file
    if (!(_file = fopen(filename.c_str(), "rb"))) {

        LOG(Error) << "Failed to load texture file " << filename;
        return false;
    }

    // If less than 54 bytes are read, problem
    if (fread(header, 1, 54, _file) != 54) {
        LOG(Error) << "Not a correct BMP file\n";
        fclose(_file);
        return false;
    }
	
    // A BMP files always begins with "BM"
    if (header[0] != 'B' || header[1] != 'M') {
        LOG(Error) << "Not a correct BMP file\n";
        fclose(_file);
        return false;
    }

    // Make sure this is a 24bpp file 1octet per pixel
    if (*reinterpret_cast<int*>(&(header[0x1E])) != 0) { printf("Incorrect BMP file\n");    fclose(_file); return false; }
    if (*reinterpret_cast<int*>(&(header[0x1C])) != 24) { printf("Incorrect BMP file\n");    fclose(_file); return false; }

    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *reinterpret_cast<int*>(&(header[0x12]));
    height = *reinterpret_cast<int*>(&(header[0x16]));
    int _format = 0;

    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

    // Create buffer
    data = new unsigned char[imageSize];
    fread(data, 1, imageSize, _file);
    fclose(_file);
	
    // Generate texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete[] data;

    // Generate MipMap
    glGenerateMipmap(GL_TEXTURE_2D);

    // Configure wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Enable Linear Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    LOG(Info) << "Loaded texture " << filename;

    return true;
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