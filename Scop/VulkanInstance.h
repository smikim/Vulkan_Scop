#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanDebug.h"
#include "VulkanTools.h"
#include "VulkanPhysicalDevice.h"

namespace vks
{
	class VulkanPhysicalDevice;

	class VulkanInstance
	{
	public:
		VulkanInstance() {}
		~VulkanInstance();

		VkResult createInstance();
		VkInstance getInstance() const;

		/**
		 * @brief Tries to find the first available discrete GPU
		 * @returns A valid physical device
		 */
		VulkanPhysicalDevice& get_first_gpu();

	private:
		std::vector<const char*> get_required_surface_extensions() const;

		/**
		* @brief Queries the instance for the physical devices on the machine
		*/
		void query_gpus();

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

		/**
		* @brief The physical devices found on the machine
		*/
		std::vector<std::unique_ptr<VulkanPhysicalDevice>> gpus;
		
		

	};
}


