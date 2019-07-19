#pragma once
#include <string>
#include <functional>

struct GLFWwindow;

class Window
{
public:
	Window(bool lockCursor = false, bool maximze = false) noexcept; //Default ctor lets the user pick settings for the window
	Window(unsigned width, unsigned height, std::string title, bool fullscreen, bool resizable = true, bool lockCursor = false, bool maximize = false) noexcept;
	~Window() noexcept;
public:
	GLFWwindow* GetWindow() const noexcept;
	unsigned GetWidth() const noexcept;
	unsigned GetHeight() const noexcept;
	void EndFrame() noexcept;
	void MakeWindow(bool resizable, bool lockCursor, bool maximize) noexcept;
	bool GetKey(int glfwKey) noexcept;
	bool ShouldClose() const noexcept;
	void Close() noexcept;
	void LockCursor() noexcept;
	void UnlockCursor() noexcept;
private:
	static constexpr unsigned testWndWidth = 800u;
	static constexpr unsigned testWndHeight = 600u;
	static constexpr bool testWndFullscreen = false;
private:
	GLFWwindow* wnd;
	unsigned width, height;
	std::string title;
	bool fullscreen;
	bool resizable;
};