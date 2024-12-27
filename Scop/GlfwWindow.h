#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
//#include "Scop.h"


namespace scop {
	class Scop; // Scop 클래스 전방 선언
}

class GlfwWindow
{
public:
	GlfwWindow(uint32_t w, uint32_t h, std::string& name);
	~GlfwWindow();

	GlfwWindow(const GlfwWindow&) = delete;
	GlfwWindow& operator=(const GlfwWindow) = delete;
	
	bool shouldClose() { return glfwWindowShouldClose(_Window); }
	GLFWwindow* getGLFWwindow() const { return _Window; }

	const uint32_t getWidth() const { return _width; }
	const uint32_t getHeight() const { return _height; }
	VkExtent2D getExtent() { return { static_cast<uint32_t>(_width), static_cast<uint32_t>(_height) }; }

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	bool wasWindowResized() { return _framebufferResized; }
	void resetWindowResizedFlag() { _framebufferResized = false; }

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
private:
	void initWindow();


	GLFWwindow* _Window;

	uint32_t _width;
	uint32_t _height;

	std::string _windowName;
	bool _framebufferResized = false;
};

