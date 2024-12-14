#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanDebug.h"

namespace vks
{
	class VulkanInstance
	{
	public:
		VulkanInstance() {}
		~VulkanInstance();

		VkResult createInstance();
	private:
		std::vector<const char*> get_required_surface_extensions() const;


		VkInstance _Instance{ VK_NULL_HANDLE };
		
		std::string name = "vulkanExample";
		uint32_t apiVersion = VK_API_VERSION_1_3;

		std::vector<const char*> _instanceExtensions;
		
		std::vector<const char*> _supportedInstanceExtensions;
		std::vector<const char*> _enabledInstanceExtensions;

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

	};
}


