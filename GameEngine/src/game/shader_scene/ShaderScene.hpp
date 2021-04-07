#pragma once
#include "Shader.h"
#include <string>
using std::string;

class ShaderScene
{	
protected:
	Shader shader;

public:
	virtual void Init() =0;
	virtual void Update() =0;
	virtual void Clean() =0;
	virtual string GetName() = 0;
};
