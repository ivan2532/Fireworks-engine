#include "Transform.hpp"
#include "Shader.hpp"
#include "imguiIncludes.hpp"
#include "GameObject.hpp"
#include "Editor.hpp"
#include "Math.hpp"
#include "HierarchyWindow.hpp"
#include "InspectorWindow.hpp"
#include <iostream>

void Transform::Update() noexcept
{
	UpdateTransform();
	UpdateShaders(); //First call UpdateTransform() !!!!!
}

void Transform::DrawInspector(Editor& editor) noexcept
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		float posInput[3] = { position.x, position.y, position.z };
		float rotInput[3] = { eulerAngles.x, eulerAngles.y, eulerAngles.z };
		float scaleInput[3] = { scale.x, scale.y, scale.z };

		auto inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue;

		ImGui::Text("Position: ");
		ImGui::SameLine();
		if (ImGui::InputFloat3("##input_pos", posInput, "%.3f", inputTextFlags))
		{
			if (!pushUndoable)
			{
				startState.reset(this);
				std::cout << "Set pushUndoable." << std::endl;
				pushUndoable = true;
			}

			SetPosition(posInput[0], posInput[1], posInput[2]);

			if (pushUndoable)
			{
				std::cout << "Process pushUndoable." << std::endl;
				PushUndoable<Transform>(editor, this, startState.get(), this);
				pushUndoable = false;
			}
		}

		ImGui::Text("Rotation: ");
		ImGui::SameLine();
		if (ImGui::InputFloat3("##input_rot", rotInput, inputTextFlags))
		{
			SetEulerAngles(rotInput[0], rotInput[1], rotInput[2]);
		}

		ImGui::Text("Scale: ");
		ImGui::SameLine();
		if (ImGui::InputFloat3("##input_scale", scaleInput, inputTextFlags))
		{
			SetScale(scaleInput[0], scaleInput[1], scaleInput[2]);
		}
	}
}

void Transform::UpdateTransform() noexcept
{
	accumulatedTransform = glm::mat4(1.0f);

	if (parent)
		accumulatedTransform = parent->transform;

	if (updateAxes)
	{
		forward.x = sin(glm::radians(eulerAngles.y)) * cos(glm::radians(eulerAngles.x));
		forward.y = sin(glm::radians(eulerAngles.x));
		forward.z = -cos(glm::radians(eulerAngles.y)) * cos(glm::radians(eulerAngles.x));
		forward = glm::normalize(forward);

		right = glm::normalize(glm::cross(forward, worldUp));
		up = glm::normalize(glm::cross(right, forward));

		forward = accumulatedTransform * glm::vec4(forward, 0.0f); //w = 0.0f PURE DIRECTIONAL VECTOR
		right = accumulatedTransform * glm::vec4(right, 0.0f);
		up = accumulatedTransform * glm::vec4(up, 0.0f);

		updateAxes = false;
	}

	transform = Math::AssembleTransform(position, scale, eulerAngles);
	transform = accumulatedTransform * transform;
}

void Transform::UpdateShaders() noexcept
{
	for (auto& shader : shadersToUpdate)
	{
		shader->SetMat4x4("model", transform);
	}
}

Transform::Transform() noexcept
	:
	position(0.0f, 0.0f, 0.0f),
	eulerAngles(0.0f, 0.0f, 0.0f),
	scale(1.0f, 1.0f, 1.0f),
	transform(1.0f),
	updateAxes(true)
{
}

Transform::Transform(const Transform& rhs) noexcept
	:
	position(rhs.position),
	eulerAngles(rhs.eulerAngles),
	scale(rhs.scale),
	transform(rhs.transform),
	parent(rhs.parent),
	updateAxes(rhs.updateAxes),
	shadersToUpdate(rhs.shadersToUpdate)
{
	gameObject = rhs.gameObject;
}

Transform& Transform::operator=(const Transform& rhs) noexcept
{
	gameObject = rhs.gameObject;
	position = rhs.position;
	eulerAngles = rhs.eulerAngles;
	scale = rhs.scale;
	transform = rhs.transform;
	parent = rhs.parent;
	updateAxes = rhs.updateAxes;
	shadersToUpdate = rhs.shadersToUpdate;

	return *this;
}

Transform::Transform(Transform&& rhs) noexcept
	:
	position(std::move(rhs.position)),
	eulerAngles(std::move(rhs.eulerAngles)),
	scale(std::move(rhs.scale)),
	transform(std::move(rhs.transform)),
	parent(std::move(rhs.parent)),
	shadersToUpdate(std::move(rhs.shadersToUpdate)),
	updateAxes(std::move(rhs.updateAxes))
{
	gameObject = std::move(rhs.gameObject);
}

Transform& Transform::operator=(Transform&& rhs) noexcept
{
	gameObject = std::move(rhs.gameObject);
	position = std::move(rhs.position);
	eulerAngles = std::move(rhs.eulerAngles);
	scale = std::move(rhs.scale);
	transform = std::move(rhs.transform);
	parent = std::move(rhs.parent);
	shadersToUpdate = std::move(rhs.shadersToUpdate);
	updateAxes = std::move(rhs.updateAxes);

	return *this;
}

void Transform::SetPosition(float x, float y, float z) noexcept
{
	SetPosition(glm::vec3(x, y, z));
}

void Transform::SetPosition(const glm::vec3& value) noexcept
{
	position = value;
	updateAxes = true;
}

glm::vec3 Transform::GetPosition() const noexcept
{
	return position;
}

void Transform::SetEulerAngles(float x, float y, float z) noexcept
{
	SetEulerAngles(glm::vec3(x, y, z));
}

void Transform::SetEulerAngles(const glm::vec3& value) noexcept
{
	eulerAngles = value;
	updateAxes = true;
}

glm::vec3 Transform::GetEulerAngles() const noexcept
{
	return eulerAngles;
}

void Transform::SetScale(float x, float y, float z) noexcept
{
	SetScale(glm::vec3(x, y, z));
}

void Transform::SetScale(const glm::vec3& value) noexcept
{
	scale = value;
	updateAxes = true;
}

glm::vec3 Transform::GetScale() const noexcept
{
	return scale;
}

glm::vec3 Transform::GetForward() const noexcept
{
	return forward;
}

glm::vec3 Transform::GetRight() const noexcept
{
	return right;
}

glm::vec3 Transform::GetUp() const noexcept
{
	return up;
}

void Transform::SetTransformation(glm::mat4 transformation, bool subtractParentTransform) noexcept
{
	if (!parent)
		subtractParentTransform = false;

	//p = parent
	glm::vec3 t, pt = glm::zero<glm::vec3>(); //Translation
	glm::vec3 s, ps = glm::zero<glm::vec3>(); //Scale
	glm::mat4 r, pr = glm::mat4(1.0f); //Rotation

	if (subtractParentTransform)
	{
		transformation = glm::inverse(accumulatedTransform) * transformation;
	}

	Math::DecomposeTransform(transformation, &t, &s, &r);

	glm::vec3 euler = Math::EulerAnglesFromRotation(r);

	SetPosition(t);
	SetEulerAngles(euler);
	SetScale(s);
}

glm::mat4 Transform::GetTransformation() const noexcept
{
	return transform;
}

void Transform::Translate(float x, float y, float z) noexcept
{
	Translate(glm::vec3(x, y, z));
}

void Transform::Translate(const glm::vec3& value) noexcept
{
	position += value;
	updateAxes = true;
}

void Transform::Rotate(float x, float y, float z) noexcept
{
	Rotate(glm::vec3(x, y, z));
}

void Transform::Rotate(const glm::vec3& value) noexcept
{
	eulerAngles += value;
	updateAxes = true;
}

void Transform::Scale(float x, float y, float z) noexcept
{
	Scale(glm::vec3(x, y, z));
}

void Transform::Scale(const glm::vec3& value) noexcept
{
	scale += value;
	updateAxes = true;
}

Transform* Transform::GetChild(int i) const noexcept
{
	return children[i];
}

Transform* Transform::GetParent() const noexcept
{
	return parent;
}

void Transform::SetParent(Transform* p) noexcept
{
	parent = p;

	if (parent)
		parent->children.push_back(this);
}

void Transform::AddShaderToUpdate(Shader* shader) noexcept
{
	shadersToUpdate.push_back(shader);
}

void Transform::DrawHierarchy(Editor& editor, int& nodeIndexCount) const noexcept
{
	auto hierarchyWindow = editor.GetEditorWindow<HierarchyWindow>().value();
	const int currentIndex = nodeIndexCount;
	nodeIndexCount++;

	auto nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (children.size() == 0)
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;

	if (currentIndex == hierarchyWindow->GetSelectedHierarchy())
		nodeFlags |= ImGuiTreeNodeFlags_Selected;

	bool open = ImGui::TreeNodeEx((void*)(intptr_t)currentIndex, nodeFlags, gameObject->GetName().c_str());

	if (ImGui::IsItemClicked())
	{
		hierarchyWindow->SetSelectedHierarchy(currentIndex);
		editor.GetEditorWindow<InspectorWindow>().value()->SetSelectedObject(gameObject);
	}

	if (open)
	{
		for (auto& child : children)
			child->DrawHierarchy(editor, nodeIndexCount);

		ImGui::TreePop();
	}
}
