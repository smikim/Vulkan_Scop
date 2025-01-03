#include "Scop.h"
#include "KeyboardMovementController.h"

namespace scop
{
	Scop::Scop()
	{

	}

	Scop::~Scop()
	{
		deleteScopObjects();
	}

	void Scop::run()
	{
		KeyboardMovementController keyboardController{};
		
		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!_window.shouldClose()) {
			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime =
				std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			frameTime = std::min(frameTime, (float)MAX_FRAME_TIME);

			glfwPollEvents();
			keyboardController.moveObjects(_window.getGLFWwindow(), frameTime, ScopObjects);
			keyboardController.switchColoring(_window.getGLFWwindow(), ScopObjects);

			update();
			render();
		}

		vkDeviceWaitIdle(_renderer.getVulkanDevice()->getLogicalDevice());
	}

	void Scop::prepare(std::string& filename)
	{
		_renderer.initVulkan();
		try {
			createScopObject(filename);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Failed to prepare ScopObject: " << e.what() << std::endl;
		}

	}

	void Scop::update()
	{
		for (size_t i = 0; i < ScopObjects.size(); i++)
		{
			ScopObjects[i]->Run();
		}
	}

	void Scop::render()
	{

		VkResult result = _renderer.beginRender();
		if (result != VK_SUCCESS) return;
		_renderer.beginRenderPass();
		for (size_t i = 0; i < ScopObjects.size(); i++)
		{
			ScopObjects[i]->Render();
		}
		_renderer.endRenderPass();
		result = _renderer.endRender();
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			return ;
	}

	ScopObject* Scop::createScopObject(std::string& filename)
	{
		ScopObject* obj = new ScopObject;
		
		if (!obj->Initialize(this, filename))
		{
			delete obj;
			throw std::runtime_error("Failed to initialize ScopObject with filename: " + filename);
		}
		
		ScopObjects.push_back(obj);
		obj->_transform.translation = { .0f, .0f, 0.0f };
		obj->_transform.scale = { 0.5f, 0.5f, 0.5f };
		return obj;
	}

	void Scop::deleteScopObjects()
	{
		for (ScopObject* obj : ScopObjects) {
			delete obj;
		}
		ScopObjects.clear();
	}
	

}
