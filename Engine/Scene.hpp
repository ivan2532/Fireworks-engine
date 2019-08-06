#pragma once
#include "GameObject.hpp"
#include <string>
#include <vector>
#include <memory>

class Engine;

class Scene
{
	friend class Editor;
	friend class HierarchyWindow;
public:
	Scene(Engine& rEngine, const std::string& name) noexcept;
public:
	std::string GetName() const noexcept;
	void AddSceneObject(const GameObject& go) noexcept;
	virtual void Update() noexcept;
	void CheckObjectDelete() noexcept;
	void UpdateScene();
public:
	void OnMouseMove(float x, float y) noexcept;
	void OnWindowResize(int width, int height) noexcept;
public:
	Engine& engine;
protected:
	std::string name;
	std::vector<GameObject> sceneObjects;
};