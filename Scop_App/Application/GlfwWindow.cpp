#include "GlfwWindow.hpp"
#include "../App/Scop.hpp"

GlfwWindow::GlfwWindow(uint32_t w, uint32_t h, std::string& name)
	: _width(w), _height(h), _windowName(name)
{
	initWindow();
}

GlfwWindow::~GlfwWindow()
{
	glfwDestroyWindow(_Window);
	glfwTerminate();
}

void GlfwWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto Window = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
	Window->_framebufferResized = true;
	Window->_width = width;
	Window->_height = height;
}

void GlfwWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void) scancode;
	(void) mods;
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			return glfwSetWindowShouldClose(window, GLFW_TRUE);
		default:
			break;
		}
	}
}

void GlfwWindow::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	
	// TO DO
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	_Window = glfwCreateWindow(_width, _height, _windowName.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(_Window, this);
	glfwSetFramebufferSizeCallback(_Window, framebufferResizeCallback);
	glfwSetKeyCallback(_Window, keyCallback);
}
