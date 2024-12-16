#include "VulkanQueue.h"
#include "VulkanDevice.h"

namespace vks
{
	VulkanQueue::VulkanQueue(VulkanDevice& device, uint32_t family_index, VkQueueFamilyProperties properties, VkBool32 can_present, uint32_t index)
		: _device{ device },
		family_index{ family_index },
		index{ index },
		properties{ properties },
		can_present{ can_present }
	{
		vkGetDeviceQueue(_device.getLogicalDevice(), family_index, index, &_Queue);
	}

	const VkQueueFamilyProperties& VulkanQueue::get_properties() const
	{
		return properties;
	}

	uint32_t VulkanQueue::get_family_index() const
	{
		return family_index;
	}

	VkQueue VulkanQueue::get_queue() const
	{
		return _Queue;
	}

}


