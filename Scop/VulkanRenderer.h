#pragma once
#include "VulkanInstance.h"
#include "GlfwWindow.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"


namespace vks
{

	class VulkanRenderer
	{
	public:
		VulkanRenderer(GlfwWindow& window);
		~VulkanRenderer();

		bool initVulkan();

	private:
		// TODO
		void getEnabledFeatures();
		void create_surface();

		void add_device_extension(const char* extension, bool optional = false)
		{
			device_extensions[extension] = optional;
		}


		std::unordered_map<const char*, bool> const& get_device_extensions() const
		{
			return device_extensions;
		}

		uint32_t _width;
		uint32_t _height;

		vks::VulkanInstance _instance;
		GlfwWindow& _window;
		VkSurfaceKHR _Surface{ VK_NULL_HANDLE };
		vks::VulkanDevice* _vulkanDevice;

		/** @brief Set of device extensions to be enabled for this example and whether they are optional (must be set in the derived constructor) */
		std::unordered_map<const char*, bool> device_extensions;

		// Wraps the swap chain to present images (framebuffers) to the windowing system
		std::unique_ptr<VulkanSwapChain> _swapChain;

	};
}