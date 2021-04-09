#include "ShaderScene.hpp"

#include "Transform.hpp"

ShaderScene::ShaderScene()
{
}

Transform ShaderScene::GetTransform() const
{
	return transform;
}
