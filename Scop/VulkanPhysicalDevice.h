#pragma once

#include "VulkanInstance.h"

namespace vks
{
	class VulkanInstance;

	class VulkanPhysicalDevice
	{
	public:
		VulkanPhysicalDevice(VulkanInstance& instance, VkPhysicalDevice physical_device);

		VulkanPhysicalDevice(const VulkanPhysicalDevice&) = delete;

		VulkanPhysicalDevice(VulkanPhysicalDevice&&) = delete;

		VulkanPhysicalDevice& operator=(const VulkanPhysicalDevice&) = delete;

		VulkanPhysicalDevice& operator=(VulkanPhysicalDevice&&) = delete;

		const VkPhysicalDeviceFeatures get_device_features() const;

		const VkPhysicalDeviceProperties& get_device_properties() const;

		const std::vector<VkQueueFamilyProperties>& get_queue_family_properties() const;

		bool has_high_priority_graphics_queue() const
		{
			return _high_priority_graphics_queue;
		}

		bool is_extension_supported(const std::string& extension) const;


		/**
		 * @brief Used at logical device creation to pass the extensions feature chain to vkCreateDevice
		 * @returns A void pointer to the start of the extension linked list
		 */
		void* get_extension_feature_chain() const;

		const VkPhysicalDeviceFeatures get_requested_features() const;

		const VkPhysicalDevice get_physical_device() const;

		VkBool32 is_present_supported(VkSurfaceKHR surface, uint32_t queue_family_index) const;

		uint32_t getMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties, VkBool32* memTypeFound = nullptr) const;

	private:
		// Handle to the Vulkan instance
		VulkanInstance& _Instance;

		// Handle to the Vulkan physical device
		VkPhysicalDevice _PhysicalDevice{ VK_NULL_HANDLE };
		// Stores physical device properties (for e.g. checking device limits)
		VkPhysicalDeviceProperties _DeviceProperties{};
		// Stores the features available on the selected physical devisce (for e.g. checking if a feature is available)
		VkPhysicalDeviceFeatures _DeviceFeatures{};
		// Stores all available memory (type) properties for the physical device
		VkPhysicalDeviceMemoryProperties _DeviceMemoryProperties{};
		/** @brief Queue family properties of the physical device */
		std::vector<VkQueueFamilyProperties> _queueFamilyProperties;
		
		bool _high_priority_graphics_queue{};

		/** @brief List of extensions supported by the device */
		std::vector<std::string> _supportedExtensions;

		// The extension feature pointer
		void* _last_requested_extension_feature{ nullptr };

		// The features that will be requested to be enabled in the logical device
		VkPhysicalDeviceFeatures _requested_features{};
	};
}

