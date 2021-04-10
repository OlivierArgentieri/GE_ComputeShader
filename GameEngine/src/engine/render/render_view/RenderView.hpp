#pragma once
#include <functional>
#include <xstring>
#include <ext/matrix_float4x4.hpp>

#include "FrameBufferObject.hpp"

class RenderView
{
	
protected:
	//FrameBufferObject* fbo = FrameBufferObject::GetInstance();
	FrameBufferObject fbo;

public:
	void Render(float _dt, glm::mat4 _mvp, const char* _idOfView);

	virtual void OverrideMeAndFillMeWithOglStuff(float _dt, glm::mat4 _mvp) = 0;
};
