#pragma once
#include <string>
#include <functional>
#include <bitset>

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
	void SetWidth(int value) noexcept;
	unsigned GetHeight() const noexcept;
	void SetHeight(int value) noexcept;
	void EndFrame() noexcept;
	void MakeWindow(bool resizable, bool lockCursor, bool maximize) noexcept;
public:
	void KeyCallback(int key, int scancode, int action, int mods) noexcept;
	bool GetKey(int glfwKey, bool respectCapture = true) noexcept;
	bool GetKeyDown(int key, bool respectCapture = true) noexcept;
	bool GetKeyUp(int key, bool respectCapture = true) noexcept;
	bool GetMouseButton(int glfwMouseButton, bool respectCapture = true) noexcept;
public:
	bool ShouldClose() const noexcept;
	void Close() noexcept;
	void LockCursor() noexcept;
	void UnlockCursor() noexcept;
	void MakeFramebuffer(unsigned width, unsigned height) noexcept;
	unsigned GetBufferWidth() const noexcept;
	unsigned GetBufferHeight() const noexcept;
	void BindFramebuffer() noexcept;
	void UnbindFramebuffer() noexcept;
	unsigned GetColorBuffer() noexcept;
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
	unsigned bufferWidth = 0;
	unsigned bufferHeight = 0;
	unsigned frameBuffer = 0;
	unsigned colorBuffer = 0;
	unsigned depthBuffer = 0;
	std::bitset<348> keyDown;
	std::bitset<348> keyUp;
};