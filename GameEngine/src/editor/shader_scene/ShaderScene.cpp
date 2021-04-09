#include "ShaderScene.hpp"

#include <gtc/type_ptr.hpp>

#include "imgui.h"
#include "Transform.hpp"

ShaderScene::ShaderScene():pivot(0), angle(0)
{
}

void ShaderScene::UpdateSettingsUI()
{
	std::string _ID = std::string(GetName()) + "_settings";
	ImGui::Begin(_ID.c_str());
		UpdateTransformEditorUI();
		UpdateShaderEditorUI();
	ImGui::End();
}

void ShaderScene::UpdateTransformEditorUI()
{
	ImGui::SliderAngle("slider angle", &angle);
	ImGui::SliderFloat3("Pivot", glm::value_ptr(pivot), 0.0f, 1.0f);
	transform.SetRotation(glm::degrees(angle), pivot);
}

void ShaderScene::UpdateShaderEditorUI()
{
	ImGui::BeginTabBar("Shaders Tab");
	if (ImGui::BeginTabItem("Vertex Shader"))
	{
		if (ImGui::Button("Reload Vertex Shader"))
		{
			OnReloadVertexShader();
		}
		ImGui::InputTextMultiline("vertexShader", &vertexShader.shaderBuffer[0], vertexShader.shaderBuffer.length() * 2, ImVec2(800, 500), ImGuiInputTextFlags_Multiline);
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Fragment Shader"))
	{
		if (ImGui::Button("Reload Fragment Shader"))
		{
			OnReloadFragmentShader();
		}
		ImGui::InputTextMultiline("fragmentShader", &fragmentShader.shaderBuffer[0], fragmentShader.shaderBuffer.length() * 2, ImVec2(800, 500), ImGuiInputTextFlags_Multiline);
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Compute Shader"))
	{
		if (ImGui::Button("Reload Compute shader"))
		{
			OnReloadFragmentShader();
		}
		ImGui::InputTextMultiline("computeShader", &computeShader.shaderBuffer[0], computeShader.shaderBuffer.length() * 2, ImVec2(800, 500), ImGuiInputTextFlags_Multiline);
		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();
}

void ShaderScene::OnReloadVertexShader()
{
	LOG(Warning) << "OnReloadVertexShader Not Implemented !";
}

void ShaderScene::OnReloadFragmentShader()
{
	LOG(Warning) << "OnReloadFragmentShader Not Implemented !";
}

void ShaderScene::OnReloadComputeShader()
{
	LOG(Warning) << "OnReloadComputeShader Not Implemented !";
}

Transform ShaderScene::GetTransform() const
{
	return transform;
}
