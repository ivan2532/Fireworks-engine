#include "OpenGLIncludes.hpp"
#include "AssetPreview.hpp"
#include "Model.hpp"
#include <iostream>

void AssetPreview::GeneratePreviewImage(std::ofstream& outputStream, Model& model) noexcept
{
	glGenFramebuffers(1u, &previewBuffer);
	glGenTextures(1u, &colorBuffer);
	glGenTextures(1u, &depthBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, previewBuffer);

	glBindTexture(GL_TEXTURE_2D, depthBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 128, 128, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, colorBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << std::endl << std::endl << "PREVIEW ERROR: Framebuffer incomplete!" << std::endl << std::endl;

	//previewShader = std::make_unique<Shader>("UnlitTextureVS.glsl", "UnlitTextureFS.glsl");
	//previewShader->Use();

	//model.LoadCPU();
	//model.LoadGPU();

	glClearColor(0.4f, 0.4f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//model.Draw(*previewShader);
	glfwSwapBuffers(glfwGetCurrentContext());

	std::vector<uint8_t> previewData(128 * 128 * 3);

	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glGetTexImage(GL_TEXTURE_2D, 0u, GL_RGB, GL_UNSIGNED_BYTE, &previewData[0]);
	glBindTexture(GL_TEXTURE_2D, 0);

	for (auto it = previewData.begin(); it != previewData.end(); ++it)
	{
		//std::cout << *it << std::endl;
		outputStream << std::move(*it);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDeleteFramebuffers(1u, &previewBuffer);
	glDeleteTextures(1u, &colorBuffer);
	glDeleteTextures(1u, &depthBuffer);
}
