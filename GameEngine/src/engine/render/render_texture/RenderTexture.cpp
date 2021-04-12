#include "RenderTexture.hpp"
#include <string>

#include "imgui.h"


void RenderTexture::Render()
{
	static bool _useless = true;
	std::string _name = std::string(GetName()) + "_texture";
	ImGui::Begin(_name.c_str(), &_useless, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);
	ImGui::TextColored(ImVec4(0.45f, 0.55f, 0.60f, 1.00f), "Render Texture View");
	ImGui::SetWindowPos(ImVec2(600, 18));
	ImGui::SetWindowSize(ImVec2(350, 300));

	std::string _childName = _name + "_render";
	ImGui::BeginChild(_childName.c_str());
	ImVec2 wsize = ImGui::GetWindowSize();
	ImGui::Image(reinterpret_cast<ImTextureID>(GetTextureToRender()), wsize, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::EndChild();
	ImGui::End();
}
