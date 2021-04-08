#pragma once
#include <functional>
#include <xstring>
#include <ext/matrix_float4x4.hpp>

#include "FrameBufferObject.hpp"

class RenderView
{
	/** def to callback render method */
	typedef void(*callback_function)(float _dt, glm::mat4 _mvp); 

private:
	FrameBufferObject fbo;
	callback_function callback;
public:
	void Render(float _dt, glm::mat4 _mvp, const char* _idOfView);

public:
	void Register(callback_function _renderMethod);
};
