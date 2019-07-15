#include "Scene.hpp"

Scene::Scene(const std::string& n) noexcept
	:
	name(n),
	sceneObjects()
{
}

std::string Scene::GetName() const noexcept
{
	return name;
}

GameObject* Scene::AddSceneObject(const std::string& name) noexcept
{
	sceneObjects.push_back(std::make_unique<GameObject>(name));
	return sceneObjects.back().get();
}

void Scene::OnMouseMove(float x, float y) noexcept
{
	for (auto& object : sceneObjects)
	{
		for (auto& component : object->components)
			component->OnMouseMove(x, y);
	}
}

void Scene::Update() noexcept
{
	for (auto& object : sceneObjects)
		object->Update();
}
