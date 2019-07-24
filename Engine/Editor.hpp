#pragma once
#include "imguiIncludes.hpp"
#include "GameObject.hpp"
#include "ImGuizmo/ImGuizmo.h"
#include "EditorWindow.hpp"
#include "EditorWindowIncludes.hpp"
#include <memory>

class Engine;
class Transform;

class Editor
{
	friend HierarchyWindow;
	friend InspectorWindow;
	friend SceneViewWindow;
public:
	Editor(Engine&) noexcept;
	Editor(const Editor&) = delete;
	Editor& operator=(const Editor&) = delete;
	Editor(Editor&&) = delete;
	Editor&& operator=(Editor&&) = delete;
private:
	void SpawnWindows() noexcept;
public:
	void DrawDockSpace() noexcept;
	void DrawGUI() noexcept;
	//Hierarchy
	void DrawHierarchyUI() noexcept;
	int GetSelectedHierarchy() const noexcept;
	void SetSelectedHierarchy(int value) noexcept;
	GameObject* GetSelectedObject() const noexcept;
	void SetSelectedObject(GameObject* value) noexcept;
	//Inspector
	void DrawInspectorUI() noexcept;
	//Asset explorer
	void DrawAssetExplorerUI() noexcept;
	//Scene view
	void DrawSceneView() noexcept;
	//Menu
	void DrawMenuBar() noexcept;
	//Gizmo
	void DrawGizmo() noexcept;
public:
	float GetSceneViewAspectRatio() const noexcept;
private:
	Engine& engine;
	std::vector<std::unique_ptr<EditorWindow>> editorWindows;
	//Hierarchy variables
	GameObject* selectedObject = nullptr;
	int nodeIndexCount = 0;
	int selectedHierarchy;
	bool sceneViewFocused = false;
	bool gameViewFocused = false;
	//Scene view variables
	float sceneViewAspectRatio;
	ImVec2 bottomLeftSceneView, topRightSceneView;
	//Gizmo variables
	unsigned translateImage, rotateImage, scaleImage;
	ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE currentMode = ImGuizmo::LOCAL;
	float gizmoMatrix[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
	Transform* gizmoTransform = nullptr;
	bool drawGizmo = false;
	//Paddings
	float dockspacePadding = 0.0f;
};