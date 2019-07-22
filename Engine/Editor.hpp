#pragma once
#include "imguiIncludes.hpp"
#include "GameObject.hpp"

class Engine;

class Editor
{
public:
	Editor(Engine& engine) noexcept;
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
	void DrawMenu() noexcept;
private:
	Engine& engine;
	//Hierarchy variables
	GameObject* selectedObject;
	int nodeIndexCount;
	int selectedHierarchy;
	bool sceneViewFocused = false;
	bool gameViewFocused = false;
	//Gizmo variables
	int transformationMode = 0; // 0 - translation, 1 - rotation, 2 - scale
	unsigned translateImage, rotateImage, scaleImage;
	//Paddings
	float dockspacePadding = 0.0f;
};