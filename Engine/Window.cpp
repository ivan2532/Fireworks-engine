#include "Window.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>

#define DETAILED_LOGGING

Window::Window(bool lc) noexcept
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

		MakeWindow(resizable, lc);

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

	MakeWindow(resizable, lc);
}

Window::Window(unsigned w, unsigned h, std::string t, bool fs, bool r, bool lc) noexcept
	:
	width(w),
	height(h),
	title(t),
	fullscreen(fs),
	resizable(r)
{
	MakeWindow(resizable, lc);
}

Window::~Window() noexcept
{
#ifdef DETAILED_LOGGING
	std::cout << "Destructing window.";
#endif
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

unsigned Window::GetHeight() const noexcept
{
	return height;
}

void Window::EndFrame() noexcept
{
	glfwSwapBuffers(wnd);
	glfwPollEvents();
}

void Window::MakeWindow(bool r, bool lc) noexcept
{
#ifdef DETAILED_LOGGING
	std::cout << std::endl << "------------------STARTING WINDOW CREATION" << std::endl;
	std::cout << "Initializing GLFW." << std::endl;
#endif
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW!";
		getchar();
		exit(-1);
	}

#ifdef DETAILED_LOGGING
	std::cout << "Setting OpenGL version." << std::endl;
#endif
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

#ifdef DETAILED_LOGGING
	std::cout << "Setting OpenGL profile." << std::endl;
#endif
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef DETAILED_LOGGING
	std::cout << "Creating window." << std::endl;
#endif
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

#ifdef DETAILED_LOGGING
	std::cout << "Setting window to current context." << std::endl;
#endif
	glfwMakeContextCurrent(wnd);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!";
		glfwTerminate();
		getchar();
		exit(-1);
	}

#ifdef DETAILED_LOGGING
	std::cout << "Setting viewport." << std::endl;
#endif
	glViewport(0, 0, width, height);

#ifdef DETAILED_LOGGING
	std::cout << "Setting resize callback." << std::endl;
#endif
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

#ifdef DETAILED_LOGGING
	std::cout << "Enabling face culling." << std::endl;
#endif
	glEnable(GL_CULL_FACE);

#ifdef DETAILED_LOGGING
	std::cout << "Enabling depth buffer." << std::endl;
#endif
	glEnable(GL_DEPTH_TEST);

#ifdef DETAILED_LOGGING
	std::cout << "Enabling v-sync." << std::endl;
#endif
	//INSERT V-SYNC HERE

#ifdef DETAILED_LOGGING
	std::cout << "Locking cursor if needed." << std::endl;
#endif
	if (lc)
		LockCursor();

#ifdef DETAILED_LOGGING
	std::cout << "------------------WINDOW CREATION SUCCESSFULLY ENDED" << std::endl << std::endl;
#endif
}

bool Window::GetKey(int key) noexcept
{
	return glfwGetKey(wnd, key);
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
