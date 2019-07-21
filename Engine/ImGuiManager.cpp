#include "ImGuiManager.hpp"
#include "imgui/imgui.h"

ImGuiManager::ImGuiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
}

ImGuiManager::~ImGuiManager()
{
	ImGui::DestroyContext();
}
