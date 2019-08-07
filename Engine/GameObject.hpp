#pragma once
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <utility>

class Component;
class Scene;
class Transform;

class GameObject
{
	friend class Scene;
	friend class Editor;
	friend class Transform;
	friend class InspectorWindow;
public:
	GameObject(const std::string& name) noexcept;
	~GameObject() = default;
	GameObject(const GameObject&) noexcept;
	GameObject& operator=(const GameObject&) noexcept;
	GameObject(GameObject&&) noexcept;
	GameObject& operator=(GameObject&&) noexcept;
public:
	std::string GetName() const noexcept;
	void SetName(const std::string&) noexcept;
	void Update() noexcept;

	template<class T>
	T* AddComponent(std::unique_ptr<T> component) noexcept;

	template<class T, typename... Args>
	T* AddComponent(Args&&... args) noexcept;

	template<class T>
	std::optional<T*> GetComponent() const noexcept;

	void Delete(bool deleteChildren = true) noexcept;
public:
	GameObject& AddChild(const GameObject& child) noexcept;
	GameObject& AddChild(GameObject&& child) noexcept;
	std::vector<GameObject>& GetChildren() noexcept;

	GameObject& AddToScene(Scene& scene, Transform* parent = nullptr) noexcept;
private:
	std::string name;
	std::vector<std::unique_ptr<Component>> components; //TODO: Maybe switch everything in the engine from vector to list?
	std::vector<GameObject> childrenObjects;
	bool deleteFlag = false;
};

template<class T>
T* GameObject::AddComponent(std::unique_ptr<T> component) noexcept
{
	components.push_back(std::move(component));
	components.back()->SetObject(this);
	components.back()->Initialize();
	return dynamic_cast<T*>(components.back().get());
}

template<typename T, class ...Args>
T* GameObject::AddComponent(Args&&... args) noexcept
{
	components.push_back(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
	components.back()->SetObject(this);
	components.back()->Initialize();
	return dynamic_cast<T*>(components.back().get());
}

template<class T>
std::optional<T*> GameObject::GetComponent() const noexcept
{
	for (auto& component : components)
		if (auto check = dynamic_cast<T*>(component.get()))
			return check;

	return {};
}
