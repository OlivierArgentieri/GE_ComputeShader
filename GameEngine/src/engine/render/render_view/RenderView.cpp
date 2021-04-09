#include "RenderView.hpp"

#include <string>

#include "imgui.h"

void RenderView::Render(float _dt, glm::mat4 _mvp, const char* _idOfView)
{
	static bool _useless = true;
	ImGui::Begin(_idOfView, &_useless, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);
	ImGui::TextColored(ImVec4(0.45f, 0.55f, 0.60f, 1.00f), "Render View");
	ImGui::SetWindowPos(ImVec2(204, 18));
		std::string _childName = std::string(_idOfView) + "_render";
		ImGui::BeginChild(_childName.c_str());
		
			GLuint _fboTextureID = fbo.CreateRenderTexture();

			OverrideMeAndFillMeWithOglStuff(_dt, _mvp);
			fbo.EndFBO();
			ImVec2 wsize = ImGui::GetWindowSize();
			ImGui::Image(reinterpret_cast<ImTextureID>(_fboTextureID), wsize, ImVec2(0, 1), ImVec2(1, 0));
		 ImGui::EndChild();
	ImGui::End();
}
