#include "Scop.h"
  
namespace scop
{
	Scop::Scop()
	{

	}

	Scop::~Scop()
	{
	}

	void Scop::run()
	{
		while (!_window.shouldClose()) {

			glfwPollEvents();

			_renderer.updateUniformBuffer();
			render();
		}

		vkDeviceWaitIdle(_renderer.getVulkanDevice()->getLogicalDevice());
	}

	void Scop::prepare()
	{
		_renderer.initVulkan();
		_renderer.buildBasicCommandBuffers();
	}

	void Scop::render()
	{

		VkResult result = _renderer.prepareFrame();
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			_renderer.buildBasicCommandBuffers();
		}

		result = _renderer.submitFrame();
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			_renderer.buildBasicCommandBuffers();
		}

	}
	

}
