#include "Editor.hpp"
#include "Engine.hpp"

Editor::Editor(Engine& eng) noexcept
	:
	engine(eng)
{
}

void Editor::DrawGUI() noexcept
{
	DrawHierarchyUI();
}

void Editor::DrawHierarchyUI() noexcept
{
	if (ImGui::Begin("Hierarchy"))
	{
		for (unsigned i = 0; i < engine.activeScene->sceneObjects.size(); i++)
		{
			auto transform = engine.activeScene->sceneObjects[i]->GetComponent<Transform>().value();

			if (transform->GetParent() == nullptr)
			{
				transform->DrawHierarchy();
			}
		}
	}
	ImGui::End();
}
