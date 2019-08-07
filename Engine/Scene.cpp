#include "Scene.hpp"
#include "Engine.hpp"
#include "GameObject.hpp"
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

GameObject& Scene::AddSceneObject(const GameObject& go) noexcept
{
	sceneObjects.push_back(go);
	return sceneObjects.back();
}

GameObject& Scene::MoveSceneObject(GameObject&& go) noexcept
{
	sceneObjects.push_back(std::move(go));
	return sceneObjects.back();
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

	for (auto& object : sceneObjects)
		object.Update();

	Update();
}
