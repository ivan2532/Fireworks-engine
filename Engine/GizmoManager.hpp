#pragma once
#include "Transform.hpp"
#include "SceneViewWindow.hpp"
#include "ImGuizmo/ImGuizmo.h"

class Editor;

class GizmoManager
{
	friend class InspectorWindow;
public:
	GizmoManager(Editor& rEditor) noexcept;
	void DrawGizmo() noexcept;
	void DrawTransformationMenu() noexcept;
private:
	Editor& editor;
	ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE currentMode = ImGuizmo::LOCAL;
	float gizmoMatrix[16];
	Transform* gizmoTransform = nullptr;
	SceneViewWindow* sceneViewWindow;
	unsigned translateImage, rotateImage, scaleImage;
};