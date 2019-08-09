#pragma once
#include <string>
#include <list>
#include <memory>

class Engine;
class GameObject;

class Scene
{
	friend class GameObject;
	friend class Editor;
	friend class HierarchyWindow;
public:
	Scene(Engine& rEngine, const std::string& name) noexcept;
public:
	std::string GetName() const noexcept;
	GameObject& AddSceneObject(const GameObject& go) noexcept;
	GameObject& MoveSceneObject(GameObject&& go) noexcept;
	virtual void Update() noexcept {};
	void CheckObjectDelete() noexcept;
	void UpdateScene();
public:
	void OnMouseMove(float x, float y) noexcept;
	void OnWindowResize(int width, int height) noexcept;
public:
	Engine& engine;
protected:
	std::string name;
	std::list<GameObject> sceneObjects;
};