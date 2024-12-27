#include "GlfwWindow.h"
#include "scop.h"

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
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			return glfwSetWindowShouldClose(window, GLFW_TRUE);
			// Rotation
		case GLFW_KEY_1:
			return scop::Scop::buttonRotation(Keymovement::RotationInput::ROTATION_ADD_X);
		case GLFW_KEY_7:
			return scop::Scop::buttonRotation(Keymovement::RotationInput::ROTATION_SUB_X);
		case GLFW_KEY_2:
			return scop::Scop::buttonRotation(Keymovement::RotationInput::ROTATION_ADD_Y);
		case GLFW_KEY_8:
			return scop::Scop::buttonRotation(Keymovement::RotationInput::ROTATION_SUB_Y);
		case GLFW_KEY_3:
			return scop::Scop::buttonRotation(Keymovement::RotationInput::ROTATION_ADD_Z);
		case GLFW_KEY_9:
			return scop::Scop::buttonRotation(Keymovement::RotationInput::ROTATION_SUB_Z);

		// Translation
		/*
		case GLFW_KEY_W:
			return App::toggleMove(ObjectDirection::MOVE_FORWARD);
		case GLFW_KEY_S:
			return App::toggleMove(ObjectDirection::MOVE_BACKWARD);
		case GLFW_KEY_A:
			return App::toggleMove(ObjectDirection::MOVE_LEFT);
		case GLFW_KEY_D:
			return App::toggleMove(ObjectDirection::MOVE_RIGHT);
		case GLFW_KEY_SPACE:
			return App::toggleMove(ObjectDirection::MOVE_UP);
		case GLFW_KEY_LEFT_CONTROL:
			return App::toggleMove(ObjectDirection::MOVE_DOWN);

			// Reset
		case GLFW_KEY_R:
			return App::resetModel();
			
		*/
		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (key) {
			// Rotation
		case GLFW_KEY_1:
			return scop::Scop::unbuttonRotation(Keymovement::RotationInput::ROTATION_SUB_X);
		case GLFW_KEY_7:
			return scop::Scop::unbuttonRotation(Keymovement::RotationInput::ROTATION_ADD_X);
		case GLFW_KEY_2:
			return scop::Scop::unbuttonRotation(Keymovement::RotationInput::ROTATION_SUB_Y);
		case GLFW_KEY_8:
			return scop::Scop::unbuttonRotation(Keymovement::RotationInput::ROTATION_ADD_Y);
		case GLFW_KEY_3:
			return scop::Scop::unbuttonRotation(Keymovement::RotationInput::ROTATION_SUB_Z);
		case GLFW_KEY_9:
			return scop::Scop::unbuttonRotation(Keymovement::RotationInput::ROTATION_ADD_Z);

			// Translation

		/*case GLFW_KEY_W:
			return App::untoggleMove(ObjectDirection::MOVE_FORWARD);
		case GLFW_KEY_S:
			return App::untoggleMove(ObjectDirection::MOVE_BACKWARD);
		case GLFW_KEY_A:
			return App::untoggleMove(ObjectDirection::MOVE_LEFT);
		case GLFW_KEY_D:
			return App::untoggleMove(ObjectDirection::MOVE_RIGHT);
		case GLFW_KEY_SPACE:
			return App::untoggleMove(ObjectDirection::MOVE_UP);
		case GLFW_KEY_LEFT_CONTROL:
			return App::untoggleMove(ObjectDirection::MOVE_DOWN);*/

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
