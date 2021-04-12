#pragma once
#include "Shader.h"
#include <string>
#include <ext/matrix_float4x4.hpp>

#include "FrameBufferObject.hpp"
#include "Transform.hpp"

using std::string;
class Editor;

class ShaderScene
{
protected:
	Shader vertexShader;
	Shader fragmentShader;
	Shader computeShader;
	Transform transform;
	ShaderScene();
	void UpdateSettingsUI();

private:
	/** Transform Editor */
	void UpdateTransformEditorUI();
	float angle;
	glm::vec3 pivot;
	glm::vec3 scale;

protected:
	/** Shader Editor*/
	void UpdateShaderEditorUI();
	virtual void OnReloadVertexShader();
	virtual void OnReloadFragmentShader();
	virtual void OnReloadComputeShader();

public:
	virtual ~ShaderScene() = default;
	virtual void Init() =0;
	virtual void Update(float _dt, glm::mat4 _mvp) =0;
	virtual void Clean() =0;
	virtual char* GetName() = 0;
	Transform GetTransform() const;
};
