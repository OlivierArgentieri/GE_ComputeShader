#include "RenderView.hpp"

#include <string>

#include "imgui.h"

void RenderView::Render(float _dt, glm::mat4 _mvp, const char* _idOfView)
{
	ImGui::Begin(_idOfView);
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
