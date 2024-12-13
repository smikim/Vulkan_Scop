#include "GlfwWindow.h"

GlfwWindow::GlfwWindow(uint32_t w, uint32_t h, std::string& name)
	: _width(w), _height(h), _windowName(name)
{
	initWindow();
}

GlfwWindow::~GlfwWindow()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void GlfwWindow::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	
	// TO DO
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	_window = glfwCreateWindow(_width, _height, _windowName.c_str(), nullptr, nullptr);

}
