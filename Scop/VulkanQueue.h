#pragma once
#include "vulkan/vulkan.h"

namespace vks
{
	class VulkanDevice;

	class VulkanQueue
	{
	public:
		VulkanQueue(VulkanDevice& device, uint32_t family_index, VkQueueFamilyProperties properties, VkBool32 can_present, uint32_t index);

		VulkanQueue(const VulkanQueue&) = default;

		//VulkanQueue(VulkanQueue&& other);

		VulkanQueue& operator=(const VulkanQueue&) = delete;

		VulkanQueue& operator=(VulkanQueue&&) = delete;

		const VkQueueFamilyProperties& get_properties() const;

		uint32_t						get_family_index() const;
		VkQueue							get_queue() const;

	private:
		VulkanDevice& _device;

		VkQueue _Queue{ VK_NULL_HANDLE };

		uint32_t family_index{ 0 };

		uint32_t index{ 0 };

		VkBool32 can_present{ VK_FALSE };

		VkQueueFamilyProperties properties{};

	};
}
