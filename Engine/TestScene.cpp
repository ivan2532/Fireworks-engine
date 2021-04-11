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
	testModel(UINT16_MAX, "Material_ball\\material_ball.obj", "Material Ball", rEngine.defaultShader)
{
	testModel.LoadCPU();
	testModel.LoadGPU();
	testModel.GetGameObject().AddToScene(*this);

	//Nasty parent setting only for testing purposes
	//transformTest.GetComponent<Transform>().value()->SetParent(sceneObjects.front().GetComponent<Transform>().value());

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

	for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
	{
		glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
		newPos = lightPositions[i];
		engine.defaultShader.SetVec3("lightPositions[" + std::to_string(i) + "]", newPos);
		engine.defaultShader.SetVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
	}
}
