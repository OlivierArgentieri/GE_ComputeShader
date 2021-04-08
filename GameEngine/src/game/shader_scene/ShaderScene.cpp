#include "ShaderScene.hpp"

#include "Game.h"
#include "Transform.hpp"

ShaderScene::ShaderScene(Game* _game)
{
	_game->RegisterShaderScene(this);
}

Transform ShaderScene::GetTransform() const
{
	return transform;
}
