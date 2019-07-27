#pragma once
#include "imguiIncludes.hpp"
#include "ImGuizmo/ImGuizmo.h"
#include "GameObject.hpp"
#include "EditorWindow.hpp"
#include "EditorWindowIncludes.hpp"
#include "GizmoManager.hpp"
#include "Undoable.hpp"
#include <memory>
#include <optional>
#include <stack>

class Engine;
class Transform;

class Editor
{
	friend HierarchyWindow;
	friend InspectorWindow;
	friend SceneViewWindow;
	friend GizmoManager;
public:
	Editor(Engine&) noexcept;
	Editor(const Editor&) = delete;
	Editor& operator=(const Editor&) = delete;
	Editor(Editor&&) = delete;
	Editor&& operator=(Editor&&) = delete;
private:
	void SpawnWindows() noexcept;
public:
	template<class T>
	std::optional<T*> GetEditorWindow() const noexcept;
public:
	//Helper function for ImGui
	bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f) noexcept;
public:
	void DrawDockSpace() noexcept;
	void DrawGUI() noexcept;
	void DrawMenuBar() noexcept;
	void ProcessInput() noexcept;
public:
	void Undo() noexcept;
	void Redo() noexcept;
private:
	Engine& engine;
	std::vector<std::unique_ptr<EditorWindow>> editorWindows;
	std::unique_ptr<GizmoManager> gizmoManager;
private:
	std::stack<std::unique_ptr<Undoable>> undoBuffer;
	std::stack<std::unique_ptr<Undoable>> redoBuffer;
private:
	bool sceneViewFocused = false;
	bool gameViewFocused = false;
	bool drawGizmo = false;
	//Paddings
	float menuPadding = 0.0f;
	float dockspacePadding = 0.0f;
};

template<class T>
std::optional<T*> Editor::GetEditorWindow() const noexcept
{
	for (auto& window : editorWindows)
		if (auto check = dynamic_cast<T*>(window.get()))
			return check;

	return {};
}
