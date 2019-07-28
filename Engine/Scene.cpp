#include "Scene.hpp"
#include "Engine.hpp"
#include <iostream>

Scene::Scene(Engine& rEngine, const std::string& n) noexcept
	:
	engine(rEngine),
	name(n),
	sceneObjects()
{
}

std::string Scene::GetName() const noexcept
{
	return name;
}

void Scene::AddSceneObject(GameObject go, bool move) noexcept
{
	sceneObjects.push_back(std::move(go));
}

void Scene::OnMouseMove(float x, float y) noexcept
{
	for (auto& object : sceneObjects)
	{
		for (auto& component : object.components)
			component->OnMouseMove(x, y);
	}
}

void Scene::OnWindowResize(int width, int height) noexcept
{
	for (auto& object : sceneObjects)
	{
		for (auto& component : object.components)
			component->OnWindowResize(width, height);
	}
}

void Scene::Update() noexcept
{
	for (auto& object : sceneObjects)
		object.Update();
}

void Scene::CheckObjectDelete() noexcept
{
	for (auto it = sceneObjects.begin(); it != sceneObjects.end();)
	{
		if (it->deleteFlag == true)
			it = sceneObjects.erase(it);
		else
			++it;
	}
}

void Scene::UpdateScene()
{
	CheckObjectDelete();
	Update();
}
