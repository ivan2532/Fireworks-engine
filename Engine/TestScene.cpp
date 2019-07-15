#include "TestScene.hpp"
#include "ComponentIncludes.hpp"
#include "Window.hpp"
#include "GLFW/glfw3.h"

TestScene::TestScene(Window& w) noexcept
	:
	Scene("Test scene"),
	wnd(w),
	textureShader("UnlitTextureVS.glsl", "UnlitTextureFS.glsl"),
	testModel("Draenei/draenei.fbx", textureShader)
{
	auto ballObject = AddSceneObject("Material Ball");
	ballObject->AddComponent(std::make_unique<Transform>(ballObject));
	//ballObject->GetComponent<Transform>().value()->AddShaderToUpdate(std::make_unique<Shader>(textureShader));
	//ballObject->AddComponent(std::make_unique<ModelRenderer>(ballObject, "Material_ball/material_ball.obj", textureShader));

	testModel.MoveToScene(*this);

	auto cameraObject = AddSceneObject("Camera");
	cameraObject->AddComponent(std::make_unique<Transform>(cameraObject));
	cameraObject->GetComponent<Transform>().value()->Translate(0.0f, 0.0f, 20.0f);
	cameraObject->GetComponent<Transform>().value()->AddShaderToUpdate(std::make_unique<Shader>(textureShader));
	cameraObject->AddComponent(std::make_unique<Camera>(cameraObject, static_cast<float>(wnd.GetWidth()), static_cast<float>(wnd.GetHeight())));
	cameraObject->AddComponent(std::make_unique<FPCameraMovement>(cameraObject, wnd, 7.0f, 0.125f));
}

void TestScene::Update() noexcept
{
	for (auto& object : sceneObjects)
		object->Update();

	textureShader.SetVec3("lightPos", glm::vec3(sin(glfwGetTime()) * 5.0f, sin(glfwGetTime()) * 2.5f, 8.0f));
	textureShader.SetVec4("pointLight.lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	textureShader.SetFloat("pointLight.ambientStrength", 0.1f);
	textureShader.SetFloat("pointLight.shininess", 32.0f);
	textureShader.SetFloat("pointLight.specularStrength", 0.7f);
	textureShader.SetFloat("pointLight.intensity", 2.0f);
	textureShader.SetFloat("pointLight.attenuation_const", 1.0f);
	textureShader.SetFloat("pointLight.attenuation_linear", 0.045f);
	textureShader.SetFloat("pointLight.attenuation_quadratic", 0.0075f);
}
