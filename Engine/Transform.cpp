#include "Transform.hpp"
#include "Shader.hpp"

void Transform::Update() noexcept
{
	UpdateTransform(); //First call UpdateTransform() !!!!!
	UpdateShaders();
}

void Transform::UpdateTransform() noexcept
{
	transform = glm::rotate(transform, eulerAngles.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, eulerAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, eulerAngles.z, glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, scale);
	transform = glm::translate(transform, position);
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
	scale(1.0f, 1.0f, 1.0f)
{
	UpdateTransform();
}

Transform::~Transform() noexcept
{
	parent.release();
}

Transform::Transform(const Transform& lhs) noexcept
	:
	position(lhs.position),
	eulerAngles(lhs.eulerAngles),
	scale(lhs.scale),
	transform(lhs.transform),
	parent(new Transform( *(lhs.parent) ))
{
}

Transform& Transform::operator=(const Transform& lhs) noexcept
{
	position = lhs.position;
	eulerAngles = lhs.eulerAngles;
	scale = lhs.scale;
	transform = lhs.transform;
	parent.reset(new Transform( *(lhs.parent) ));
	return *this;
}

Transform::Transform(Transform&& lhs) noexcept
	:
	position(std::move(lhs.position)),
	eulerAngles(std::move(lhs.eulerAngles)),
	scale(std::move(lhs.scale)),
	transform(std::move(lhs.transform)),
	parent(std::move(lhs.parent))
{
}

Transform& Transform::operator=(Transform&& lhs) noexcept
{
	position = std::move(lhs.position);
	eulerAngles = std::move(lhs.eulerAngles);
	scale = std::move(lhs.scale);
	transform = std::move(lhs.transform);
	parent = std::move(lhs.parent);
	return *this;
}

void Transform::SetPosition(glm::vec3 value) noexcept
{
	position = value;
}

glm::vec3 Transform::GetPosition() const noexcept
{
	return position;
}

void Transform::SetEulerAngles(glm::vec3 value) noexcept
{
	eulerAngles = value;
}

glm::vec3 Transform::GetEulerAngles() const noexcept
{
	return eulerAngles;
}

void Transform::SetScale(glm::vec3 value) noexcept
{
	scale = value;
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

void Transform::AddShaderToUpdate(std::unique_ptr<Shader> shader) noexcept
{
	shadersToUpdate.push_back(std::move(shader));
}
