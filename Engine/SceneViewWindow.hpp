#pragma once
#include "EditorWindow.hpp"
#include "imguiIncludes.hpp"

class Engine;

class SceneViewWindow : public EditorWindow
{
public:
	SceneViewWindow(Editor& editor) noexcept;
public:
	void Draw() noexcept override;
	ImVec2 GetBottomLeft() const noexcept;
	ImVec2 GetTopRight() const noexcept;
	void SetRect(const ImVec2& bottomLeft, const ImVec2& topRight) noexcept;
	float GetSceneViewAspectRatio() const noexcept;
private:
	float sceneViewAspectRatio = 4.0f / 3.0f;
	ImVec2 bottomLeftSceneView, topRightSceneView;
};