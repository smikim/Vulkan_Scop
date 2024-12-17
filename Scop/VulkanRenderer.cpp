#include "VulkanRenderer.h"
#include "VulkanTools.h"

namespace vks
{
	VulkanRenderer::VulkanRenderer(GlfwWindow& window)
		: _window(window)
	{
		add_device_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	}

	vks::VulkanRenderer::~VulkanRenderer()
	{
		_swapChain->cleanup();
		delete _vulkanDevice;

		
	}

	bool VulkanRenderer::initVulkan()
	{
		VK_CHECK_RESULT(_instance.createInstance());
		create_surface();
		_vulkanDevice = new vks::VulkanDevice(_instance.get_first_gpu(), _Surface, get_device_extensions());
		_swapChain = std::make_unique<VulkanSwapChain>(_instance, *_vulkanDevice, _Surface);
		_swapChain->initSurface(_window.getGLFWwindow());
		_width = _window.getWidth();
		_height = _window.getHeight();
		_swapChain->create(_width, _height);

		return true;
	}

	void VulkanRenderer::getEnabledFeatures()
	{

	}

	void VulkanRenderer::create_surface()
	{
		if (glfwCreateWindowSurface(_instance.getInstance(), _window.getGLFWwindow(), nullptr, &_Surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface");
		}
	}

}

