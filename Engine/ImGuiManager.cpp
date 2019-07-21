#include "ImGuiManager.hpp"
#include "imgui/imgui.h"

ImGuiManager::ImGuiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsLight();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

ImGuiManager::~ImGuiManager()
{
	ImGui::DestroyContext();
}
