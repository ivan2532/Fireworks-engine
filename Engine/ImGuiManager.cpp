#include "ImGuiManager.hpp"
#include "imgui/imgui.h"

ImGuiManager::ImGuiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsLight();
}

ImGuiManager::~ImGuiManager()
{
	ImGui::DestroyContext();
}
