#pragma once
#include <functional>
#include <xstring>
#include <ext/matrix_float4x4.hpp>

#include "FrameBufferObject.hpp"

class RenderView
{
private:
	glm::vec2 position = glm::vec2(216, 18);
	glm::vec2 size = glm::vec2(350, 300);

protected:
	//FrameBufferObject* fbo = FrameBufferObject::GetInstance();
	FrameBufferObject fbo;

public:
	void Render(float _dt, glm::mat4 _mvp, const char* _idOfView);

	virtual void OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp) = 0;

protected:
	glm::vec2 GetCursorPosition();
};
