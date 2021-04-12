#pragma once
#include <glew.h>


class RenderTexture
{
public:
	RenderTexture() = default;
	RenderTexture(const RenderTexture&) = delete;

protected:
	virtual GLuint GetTextureToRender() = 0;
	virtual char* GetName() = 0;
	void Render();
};
