#pragma once
#include "Shader.h"
#include <string>
#include <ext/matrix_float4x4.hpp>
using std::string;

class ShaderScene
{	
protected:
	Shader shader;

public:
	virtual void Init() =0;
	virtual void Update(float _dt, glm::mat4 _mvp) =0;
	virtual void Clean() =0;
	virtual string GetName() = 0;
};
