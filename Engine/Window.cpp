#include "Window.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imguiIncludes.hpp"
#include <iostream>

Window::Window(bool lc, bool max) noexcept
{
	char input = 0; //Used for handling Y/N

	std::cout << "Window wizard: " << std::endl << std::endl;

	std::cout << "Use premade test window (Y/N): ";
	while ((input = getchar()) != 'Y' && input != 'N');

	if (input == 'Y')
	{
		width = testWndWidth;
		height = testWndHeight;
		title = "Test window";
		fullscreen = testWndFullscreen;
		resizable = true;

		MakeWindow(resizable, lc, max);

		return;
	}

	std::cout << "Window width: ";
	std::cin >> width;

	std::cout << "Window height: ";
	std::cin >> height;

	std::cout << "Window title: ";
	std::cin >> title;

	std::cout << "Fullscreen (Y/N): ";
	while ((input = getchar()) != 'Y' && input != 'N');

	if (input == 'Y')
		fullscreen = true;
	else
		fullscreen = false;

	if (!fullscreen)
	{
		std::cout << "Resizable (Y/N): ";
		while ((input = getchar()) != 'Y' && input != 'N');
		if (input == 'Y')
			resizable = true;
		else
			resizable = false;
	}
	else
		resizable = false;

	MakeWindow(resizable, lc, max);
}

Window::Window(unsigned w, unsigned h, std::string t, bool fs, bool r, bool lc, bool max) noexcept
	:
	width(w),
	height(h),
	title(t),
	fullscreen(fs),
	resizable(r)
{
	MakeWindow(resizable, lc, max);
}

Window::~Window() noexcept
{
	glfwDestroyWindow(wnd);
	glfwTerminate();
}

GLFWwindow* Window::GetWindow() const noexcept
{
	return wnd;
}

unsigned Window::GetWidth() const noexcept
{
	return width;
}

void Window::SetWidth(int value) noexcept
{
	width = value;
}

unsigned Window::GetHeight() const noexcept
{
	return height;
}

void Window::SetHeight(int value) noexcept
{
	height = value;
}

void Window::EndFrame() noexcept
{
	glfwSwapBuffers(wnd);
	glfwPollEvents();
}

void Window::MakeWindow(bool r, bool lc, bool max) noexcept
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW!";
		getchar();
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (!fullscreen && max)
	{
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

		const auto videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		width = videoMode->width;
		height = videoMode->height;
	}

	if (fullscreen)
		wnd = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
	else
		wnd = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (wnd == nullptr)
	{
		std::cout << "Failed to create window!";
		glfwTerminate();
		getchar();
		exit(-1);
	}

	glfwMakeContextCurrent(wnd);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!";
		glfwTerminate();
		getchar();
		exit(-1);
	}

	glViewport(0, 0, width, height);

	if (resizable)
	{
		glfwSetFramebufferSizeCallback(
			wnd,
			[](GLFWwindow* wnd, int w, int h)
			{
				glViewport(0, 0, w, h);
			}
		);
	}
	else
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	//INSERT V-SYNC HERE

	if (lc)
		LockCursor();
}

void Window::KeyCallback(int key, int scancode, int action, int mods) noexcept
{
	if (action == GLFW_PRESS)
		keyDown.set(key, 1);
	else if (action == GLFW_RELEASE)
		keyUp.set(key, 1);
}

bool Window::GetKey(int key, bool respectCapture) noexcept
{
	if (respectCapture && ImGui::GetIO().WantCaptureKeyboard)
		return false;

	return glfwGetKey(wnd, key);
}
bool Window::GetKeyDown(int key, bool respectCapture) noexcept
{
	bool result = keyDown.test(key);

	if (keyDown.test(key) && glfwGetKey(wnd, key))
		keyDown.set(key, 0);

	if (respectCapture && ImGui::GetIO().WantCaptureKeyboard)
		return false;

	return result;
}
bool Window::GetKeyUp(int key, bool respectCapture) noexcept
{
	bool result = keyUp.test(key);

	if (keyUp.test(key) && !glfwGetKey(wnd, key))
		keyUp.set(key, 0);

	if (respectCapture && ImGui::GetIO().WantCaptureKeyboard)
		return false;

	return result;
}
bool Window::GetMouseButton(int glfwMouseButton, bool respectCapture) noexcept
{
	return glfwGetMouseButton(wnd, glfwMouseButton);
}

bool Window::ShouldClose() const noexcept
{
	return glfwWindowShouldClose(wnd);
}

void Window::Close() noexcept
{
	glfwSetWindowShouldClose(wnd, GLFW_TRUE);
}

void Window::LockCursor() noexcept
{
	glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::UnlockCursor() noexcept
{
	glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::MakeFramebuffer(unsigned width, unsigned height) noexcept
{
	bufferWidth = width;
	bufferHeight = height;
	glViewport(0, 0, bufferWidth, bufferHeight);

	glGenFramebuffers(1u, &frameBuffer);
	BindFramebuffer();

	glGenTextures(1u, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bufferWidth, bufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1u, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, bufferWidth, bufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

	UnbindFramebuffer();
}

unsigned Window::GetBufferWidth() const noexcept
{
	return bufferWidth;
}

unsigned Window::GetBufferHeight() const noexcept
{
	return bufferHeight;
}

void Window::BindFramebuffer() noexcept
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void Window::UnbindFramebuffer() noexcept
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << std::endl << std::endl <<  "ERROR: Framebuffer incomplete!" << std::endl << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned Window::GetColorBuffer() noexcept
{
	return colorBuffer;
}
