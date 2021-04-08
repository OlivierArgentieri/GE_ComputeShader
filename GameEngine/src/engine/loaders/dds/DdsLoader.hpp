#pragma once
#include <cstdio>
#include <glew.h>


class DdsLoader
{
private:
	static bool FileIsValid(FILE* _file);

	DdsLoader() {}
	DdsLoader(const DdsLoader&) = delete;

public:
	static void LoadFile(const char* _path, GLuint& _outTextureID);
};
