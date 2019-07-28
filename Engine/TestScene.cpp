#include "TestScene.hpp"
#include "ComponentIncludes.hpp"
#include "Window.hpp"
#include "GLFW/glfw3.h"

TestScene::TestScene(Engine& rEngine, Window& w) noexcept
	:
	Scene(rEngine, "Test scene"),
	wnd(w),
	textureShader("UnlitTextureVS.glsl", "UnlitTextureFS.glsl"),
	testModel("Material_ball/material_ball.obj", textureShader)
{
	testModel.AddToScene(*this);

	GameObject transformTest("Test child");
	transformTest.AddComponent<Transform>();

	//Nasty parent setting only for testing purposes
	//transformTest.GetComponent<Transform>().value()->SetParent(sceneObjects.front().GetComponent<Transform>().value());
	
	AddSceneObject(std::move(transformTest));

	GameObject cameraObject("SceneCamera");
	cameraObject.AddComponent<Transform>();
	cameraObject.GetComponent<Transform>().value()->Translate(0.0f, 0.0f, 20.0f);
	cameraObject.GetComponent<Transform>().value()->AddShaderToUpdate(&textureShader);
	cameraObject.AddComponent<Camera>(engine);
	cameraObject.AddComponent<SceneCameraController>(wnd, 7.0f, 0.25f);
	AddSceneObject(std::move(cameraObject));
}

void TestScene::Update() noexcept
{
	for (auto& object : sceneObjects)
		object.Update();

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
