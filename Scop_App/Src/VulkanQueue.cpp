#include "../Includes/VulkanQueue.hpp"
#include "../Includes/VulkanDevice.hpp"

namespace vks
{
	VulkanQueue::VulkanQueue(VulkanDevice& device, uint32_t family_index, VkQueueFamilyProperties properties, VkBool32 can_present, uint32_t index)
		: _device{ device },
			family_index{ family_index },
			index{ index },
			can_present{ can_present },
			properties{ properties }
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

	VkBool32 VulkanQueue::support_present() const
	{
		return can_present;
	}

}


