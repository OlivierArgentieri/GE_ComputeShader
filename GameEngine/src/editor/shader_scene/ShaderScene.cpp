#include "ShaderScene.hpp"

#include <gtc/type_ptr.hpp>

#include "imgui.h"
#include "Transform.hpp"

ShaderScene::ShaderScene() :angle(0), pivot(1,0,0), scale(1)
{
}

void ShaderScene::UpdateSettingsUI(float _dt)
{
	std::string _ID = std::string(GetName()) + "_settings";
	ImGui::Begin(_ID.c_str());
		UpdateTransformEditorUI(_dt);
		UpdateShaderEditorUI();
	ImGui::End();
}

void ShaderScene::UpdateTransformEditorUI(float _dt)
{
	static float _speedAuto = 2;
	ImGui::SliderAngle("slider angle", &angle);
	ImGui::SameLine();
	ImGui::Checkbox("Auto", &_autoMod);
	ImGui::SliderFloat3("Pivot", glm::value_ptr(pivot), 0.0f, 1.0f);
	
	if (_autoMod)
	{
		angle += _speedAuto * _dt;

		// because : angle = angle % 360; only works in C# ? 
		if (glm::degrees(angle) > 360)
			angle = glm::radians(-360.0f);
	}

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
		ImGui::PushItemWidth(-1);
		ImGui::InputTextMultiline("vertexShader", &vertexShader.shaderBuffer[0], vertexShader.shaderBuffer.length() * 2, ImVec2(800, 500), ImGuiInputTextFlags_Multiline);
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Fragment Shader"))
	{
		if (ImGui::Button("Reload Fragment Shader"))
		{
			OnReloadFragmentShader();
		}
		ImGui::PushItemWidth(-1);
		ImGui::InputTextMultiline("fragmentShader", &fragmentShader.shaderBuffer[0], fragmentShader.shaderBuffer.length()*2 , ImVec2(800, 500), ImGuiInputTextFlags_Multiline);
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Compute Shader"))
	{
		if (ImGui::Button("Reload Compute shader"))
		{
			OnReloadComputeShader();
		}
		ImGui::PushItemWidth(-1);
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
