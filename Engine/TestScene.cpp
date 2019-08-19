#include "TestScene.hpp"
#include "ComponentIncludes.hpp"
#include "Window.hpp"
#include "GLFW/glfw3.h"
#include "Engine.hpp"
#include <iostream>

TestScene::TestScene(Engine& rEngine, Window& w) noexcept
	:
	Scene(rEngine, "Test scene"),
	wnd(w),
	testModel(UINT16_MAX, "TrexModelByJoel3d_FBX\\TrexByJoel3d.fbx", "trex_test", rEngine.defaultShader)
{
	testModel.LoadCPU();
	testModel.LoadGPU();
	testModel.GetGameObject().AddToScene(*this);

	GameObject transformTest("Test child");
	transformTest.AddComponent<Transform>();

	//Nasty parent setting only for testing purposes
	//transformTest.GetComponent<Transform>().value()->SetParent(sceneObjects.front().GetComponent<Transform>().value());
	
	AddSceneObject(std::move(transformTest));

	GameObject cameraObject("SceneCamera");
	auto transform = cameraObject.AddComponent<Transform>();
	transform->Translate(0.0f, 0.0f, 20.0f);
	transform->AddShaderToUpdate(&rEngine.defaultShader);
	cameraObject.AddComponent<Camera>(engine);
	cameraObject.AddComponent<SceneCameraController>(wnd, 7.0f, 0.25f);
	cameraObject.AddToScene(*this);
}

void TestScene::Update() noexcept
{
	engine.defaultShader.SetVec3("lightPos", glm::vec3(sin(glfwGetTime()) * 5.0f, sin(glfwGetTime()) * 2.5f, 8.0f));
	engine.defaultShader.SetVec4("pointLight.lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	engine.defaultShader.SetFloat("pointLight.ambientStrength", 0.1f);
	engine.defaultShader.SetFloat("pointLight.shininess", 32.0f);
	engine.defaultShader.SetFloat("pointLight.specularStrength", 0.7f);
	engine.defaultShader.SetFloat("pointLight.intensity", 2.0f);
	engine.defaultShader.SetFloat("pointLight.attenuation_const", 1.0f);
	engine.defaultShader.SetFloat("pointLight.attenuation_linear", 0.045f);
	engine.defaultShader.SetFloat("pointLight.attenuation_quadratic", 0.0075f);
}
