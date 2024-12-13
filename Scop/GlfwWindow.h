#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

class GlfwWindow
{
public:
	GlfwWindow(uint32_t w, uint32_t h, std::string& name);
	~GlfwWindow();

	GlfwWindow(const GlfwWindow&) = delete;
	GlfwWindow& operator=(const GlfwWindow) = delete;
	
	bool shouldClose() { return glfwWindowShouldClose(_window); }

private:
	void initWindow();


	GLFWwindow* _window;

	uint32_t _width;
	uint32_t _height;

	std::string _windowName;
};

