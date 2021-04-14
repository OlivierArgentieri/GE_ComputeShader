#include "RenderView.hpp"

#include <string>

#include "imgui.h"
#include "Window.h"

void RenderView::Render(float _dt, glm::mat4 _mvp, const char* _idOfView)
{
	static bool _useless = true;
	ImGui::Begin(_idOfView, &_useless, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);
	ImGui::TextColored(ImVec4(0.45f, 0.55f, 0.60f, 1.00f), "Render View");
	ImGui::SetWindowPos(ImVec2(position.x, position.y));
	ImGui::SetWindowSize(ImVec2(size.x, size.y));
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

glm::vec2 RenderView::GetCursorPosition()
{
	static double _x;
	static double _y;
	static glm::vec2 _test;
	static glm::vec2 _sw = Window::GetSize();
	
	Window::GetCursorPosition(_x, _y);

	//_test = glm::vec2(_x - position.x, _y - position.y);
	//_test = glm::vec2(_x + 120, _y + 25); 375 164 
	//_test = glm::vec2(_x + 120, _y + 25);

	_test = glm::vec2(_x - (position.x + size.x/2), _y - (position.y + size.y / 2));
	//_test = glm::vec2(_x - 375, _y - 174);

	_test.x = glm::clamp(_test.x, -(position.x + size.x / 2), (position.x + size.x / 2));
	_test.y = glm::clamp(_test.y, -(position.y + size.y / 2), (position.y + size.y / 2));
	_test *= 0.1;
	//LOG(Info) << (position.x + size.x / 2) /2 << "    " << _test.x;
	//LOG(Info) << (position.x + size.x / 2) /2 << "    " << _test.y;

	return _test;
}
