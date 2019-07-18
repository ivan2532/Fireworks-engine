#pragma once
#include "GameObject.hpp"
#include <string>
#include <vector>
#include <memory>

class Scene
{
	friend class Editor;
public:
	Scene(const std::string& name) noexcept;
public:
	std::string GetName() const noexcept;
	GameObject* AddSceneObject(const std::string& name) noexcept;
	void MoveSceneObject(std::unique_ptr<GameObject> go) noexcept;
	virtual void Update() noexcept;
public:
	void OnMouseMove(float x, float y) noexcept;
	void OnWindowResize(int width, int height) noexcept;
protected:
	std::string name;
	std::vector<std::unique_ptr<GameObject>> sceneObjects;
};