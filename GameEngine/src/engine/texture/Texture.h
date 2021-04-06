#pragma once

#include <string>
using std::string;

class Texture
{
private:
    unsigned int textureID;
    string filename;
    unsigned int width;
    unsigned int height;
    unsigned char* data;

public:

    Texture();
    ~Texture();

    void Unload();
    bool Load(const string& _filename);
    void UpdateInfo(int& _widthOut, int& _heightOut);
    void Use() const;

    unsigned int GetTextureID() const { return textureID; }
    inline int GetWidth() const { return width; }
    inline int GetHeight() const { return height; }
};