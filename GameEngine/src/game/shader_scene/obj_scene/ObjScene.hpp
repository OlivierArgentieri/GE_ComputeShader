#pragma once
#include "ShaderScene.hpp"

class ObjScene : public ShaderScene
{
private:

public:
	ObjScene() = default;
	ObjScene(const ObjScene&) = delete;

	void Init() override;
	void Update() override;
	void Clean() override;
	string GetName() override;
};