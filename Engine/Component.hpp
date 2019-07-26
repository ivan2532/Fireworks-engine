#pragma once

class GameObject;

class Component
{
	friend class Scene;
	friend class GameObject;
public:
	Component() = default;
public:
	GameObject* GetObject() const noexcept;
	void SetObject(GameObject* go) noexcept;
	virtual void Update() noexcept = 0;
	virtual void DrawInspector() noexcept {}
private:
	virtual void Initialize() noexcept {}
	virtual void OnMouseMove(float x, float y) noexcept {}
	virtual void OnWindowResize(int width, int height) noexcept {}
protected:
	GameObject* gameObject = nullptr;
};