#include "Transform.hpp"
#include "Shader.hpp"

void Transform::Update() noexcept
{
	UpdateTransform();
	UpdateShaders(); //First call UpdateTransform() !!!!!
}

void Transform::UpdateTransform() noexcept
{
	transform = glm::rotate(glm::mat4(1.0f), eulerAngles.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(glm::mat4(1.0f), eulerAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(glm::mat4(1.0f), eulerAngles.z, glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(glm::mat4(1.0f), scale);
	transform = glm::translate(glm::mat4(1.0f), position);
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
	transform(1.0f)
{
	UpdateTransform();
}

Transform::~Transform() noexcept
{
	parent.release();
}

Transform::Transform(const Transform& rhs) noexcept
	:
	position(rhs.position),
	eulerAngles(rhs.eulerAngles),
	scale(rhs.scale),
	transform(rhs.transform),
	parent((rhs.parent).get())
{
	shadersToUpdate.resize(rhs.shadersToUpdate.size());
	for (unsigned i = 0; i < rhs.shadersToUpdate.size(); i++)
		shadersToUpdate[i].reset((rhs.shadersToUpdate[i].get()));
}

Transform& Transform::operator=(const Transform& rhs) noexcept
{
	position = rhs.position;
	eulerAngles = rhs.eulerAngles;
	scale = rhs.scale;
	transform = rhs.transform;
	parent.reset((rhs.parent).get());

	shadersToUpdate.resize(rhs.shadersToUpdate.size());
	for (unsigned i = 0; i < rhs.shadersToUpdate.size(); i++)
		shadersToUpdate[i].reset((rhs.shadersToUpdate[i].get()));

	return *this;
}

Transform::Transform(Transform&& rhs) noexcept
	:
	position(std::move(rhs.position)),
	eulerAngles(std::move(rhs.eulerAngles)),
	scale(std::move(rhs.scale)),
	transform(std::move(rhs.transform)),
	parent(std::move(rhs.parent)),
	shadersToUpdate(std::move(rhs.shadersToUpdate))
{
}

Transform& Transform::operator=(Transform&& rhs) noexcept
{
	position = std::move(rhs.position);
	eulerAngles = std::move(rhs.eulerAngles);
	scale = std::move(rhs.scale);
	transform = std::move(rhs.transform);
	parent = std::move(rhs.parent);
	shadersToUpdate = std::move(rhs.shadersToUpdate);
	return *this;
}

void Transform::SetPosition(glm::vec3 value) noexcept
{
	position = value;
}

void Transform::SetPosition(float x, float y, float z) noexcept
{
	SetPosition(glm::vec3(x, y, z));
}

glm::vec3 Transform::GetPosition() const noexcept
{
	return position;
}

void Transform::SetEulerAngles(glm::vec3 value) noexcept
{
	eulerAngles = value;
}

void Transform::SetEulerAngles(float x, float y, float z) noexcept
{
	SetEulerAngles(glm::vec3(x, y, z));
}

glm::vec3 Transform::GetEulerAngles() const noexcept
{
	return eulerAngles;
}

void Transform::SetScale(glm::vec3 value) noexcept
{
	scale = value;
}

void Transform::SetScale(float x, float y, float z) noexcept
{
	SetScale(glm::vec3(x, y, z));
}

glm::vec3 Transform::GetScale() const noexcept
{
	return scale;
}

glm::mat4 Transform::GetTransformation() const noexcept
{
	return transform;
}

void Transform::Translate(glm::vec3 value) noexcept
{
	position += value;
}

void Transform::Rotate(glm::vec3 value) noexcept
{
	eulerAngles += value;
}

void Transform::Scale(glm::vec3 value) noexcept
{
	scale += value;
}

void Transform::AddShaderToUpdate(std::unique_ptr<Shader> shader) noexcept
{
	shadersToUpdate.push_back(std::move(shader));
}
