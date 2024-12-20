#include "VulkanDevice.h"
#include "helpers.h"

namespace vks
{
	VulkanDevice::VulkanDevice(VulkanPhysicalDevice& gpu, VkSurfaceKHR surface, std::unordered_map<const char*, bool> requested_extensions)
		: _gpu(gpu)
	{
		createLogicalDevice(surface, enabledFeatures, enabledDeviceExtensions, deviceCreatepNextChain, requested_extensions);

	}

	VulkanDevice::~VulkanDevice()
	{
		vkDestroyCommandPool(_LogicalDevice, _CommandPool, nullptr);

		vkDestroyDevice(_LogicalDevice, nullptr);
	}

	VkResult VulkanDevice::createLogicalDevice(VkSurfaceKHR surface,
		VkPhysicalDeviceFeatures enabledFeatures, 
		std::vector<const char*> enabledExtensions, 
		void* pNextChain, 
		std::unordered_map<const char*, bool> requested_extensions, 
		bool useSwapChain, 
		VkQueueFlags requestedQueueTypes)
	{
		// Prepare the device queues
		uint32_t                             queue_family_properties_count = to_u32(_gpu.get_queue_family_properties().size());
		std::vector<VkDeviceQueueCreateInfo> queue_create_infos(queue_family_properties_count, { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO });
		std::vector<std::vector<float>>      queue_priorities(queue_family_properties_count);

		for (uint32_t queue_family_index = 0U; queue_family_index < queue_family_properties_count; ++queue_family_index)
		{
			const VkQueueFamilyProperties& queue_family_property = _gpu.get_queue_family_properties()[queue_family_index];

			if (_gpu.has_high_priority_graphics_queue())
			{
				uint32_t graphics_queue_family = get_queue_family_index(VK_QUEUE_GRAPHICS_BIT);
				if (graphics_queue_family == queue_family_index)
				{
					queue_priorities[queue_family_index].reserve(queue_family_property.queueCount);
					queue_priorities[queue_family_index].push_back(1.0f);
					for (uint32_t i = 1; i < queue_family_property.queueCount; i++)
					{
						queue_priorities[queue_family_index].push_back(0.5f);
					}
				}
				else
				{
					queue_priorities[queue_family_index].resize(queue_family_property.queueCount, 0.5f);
				}
			}
			else
			{
				queue_priorities[queue_family_index].resize(queue_family_property.queueCount, 0.5f);
			}

			VkDeviceQueueCreateInfo& queue_create_info = queue_create_infos[queue_family_index];

			queue_create_info.queueFamilyIndex = queue_family_index;
			queue_create_info.queueCount = queue_family_property.queueCount;
			queue_create_info.pQueuePriorities = queue_priorities[queue_family_index].data();

		}
			
		// Check that extensions are supported before trying to create the device
		std::vector<const char*> unsupported_extensions{};
		for (auto& extension : requested_extensions)
		{
			if (is_extension_supported(extension.first))
			{
				enabledDeviceExtensions.emplace_back(extension.first);
			}
			else
			{
				unsupported_extensions.emplace_back(extension.first);
			}
		}

		if (enabledDeviceExtensions.size() > 0)
		{
			std::cout << "Device supports the following requested extensions:";
			for (auto& extension : enabledDeviceExtensions)
			{
				std::cout << "  \t{}" << extension << std::endl;
			}
		}

		if (unsupported_extensions.size() > 0)
		{
			auto error = false;
			for (auto& extension : unsupported_extensions)
			{
				auto extension_is_optional = requested_extensions[extension];
				if (extension_is_optional)
				{
					std::cerr << "Optional device extension {} not available, some features may be disabled" << extension << std::endl;
				}
				else
				{
					std::cerr << "Required device extension {} not available, cannot run" << extension << std::endl;
					error = true;
				}
			}

			if (error)
			{
				throw std::runtime_error("Extensions not present");
			}
		}

		VkDeviceCreateInfo create_info{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };

		// Latest requested feature will have the pNext's all set up for device creation.
		create_info.pNext = _gpu.get_extension_feature_chain();

		create_info.pQueueCreateInfos = queue_create_infos.data();
		create_info.queueCreateInfoCount = to_u32(queue_create_infos.size());
		create_info.enabledExtensionCount = to_u32(enabledDeviceExtensions.size());
		create_info.ppEnabledExtensionNames = enabledDeviceExtensions.data();

		const auto requested_gpu_features = _gpu.get_requested_features();
		create_info.pEnabledFeatures = &requested_gpu_features;

		VkResult result = vkCreateDevice(_gpu.get_physical_device(), &create_info, nullptr, &_LogicalDevice);

		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Cannot create device");
		}

		create_graphics_queue(surface);

		std::cout << "device command pool" << std::endl;
	
		// Create a default command pool for graphics command buffers
		_CommandPool = createCommandPool(get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT, 0).get_family_index());
		std::cout << _CommandPool << std::endl;
		std::cout << "device command pool" << std::endl;
		return result;

	}

	uint32_t VulkanDevice::get_queue_family_index(VkQueueFlagBits queueFlags)
	{
		std::vector<VkQueueFamilyProperties> queueFamilyProperties = _gpu.get_queue_family_properties();

		// Dedicated queue for compute
			// Try to find a queue family index that supports compute but not graphics
		if ((queueFlags & VK_QUEUE_COMPUTE_BIT) == queueFlags)
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
			{
				if ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
				{
					return i;
				}
			}
		}

		// Dedicated queue for transfer
		// Try to find a queue family index that supports transfer but not graphics and compute
		if ((queueFlags & VK_QUEUE_TRANSFER_BIT) == queueFlags)
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
			{
				if ((queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
				{
					return i;
				}
			}
		}

		// For other queue types or if no separate compute queue is present, return the first one to support the requested flags
		for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
		{
			if ((queueFamilyProperties[i].queueFlags & queueFlags) == queueFlags)
			{
				return i;
			}
		}

		throw std::runtime_error("Could not find a matching queue family index");

	}

	const VulkanQueue& VulkanDevice::get_queue_by_flags(VkQueueFlags required_queue_flags, uint32_t queue_index) const
	{
		for (uint32_t queue_family_index = 0U; queue_family_index < queues.size(); ++queue_family_index)
		{
			VulkanQueue const& first_queue = queues[queue_family_index][0];

			VkQueueFlags queue_flags = first_queue.get_properties().queueFlags;
			uint32_t     queue_count = first_queue.get_properties().queueCount;

			if (((queue_flags & required_queue_flags) == required_queue_flags) && queue_index < queue_count)
			{
				return queues[queue_family_index][queue_index];
			}
		}

		throw std::runtime_error("Queue not found");
		
	}
	
	bool VulkanDevice::is_extension_supported(const std::string& requested_extension) const
	{
		return _gpu.is_extension_supported(requested_extension);
	}

	void VulkanDevice::create_graphics_queue(VkSurfaceKHR surface)
	{
		uint32_t queue_family_properties_count = to_u32(_gpu.get_queue_family_properties().size());

		queues.resize(queue_family_properties_count);

		for (uint32_t queue_family_index = 0U; queue_family_index < queue_family_properties_count; ++queue_family_index)
		{
			const VkQueueFamilyProperties& queue_family_property = _gpu.get_queue_family_properties()[queue_family_index];

			VkBool32 present_supported = _gpu.is_present_supported(surface, queue_family_index);

			for (uint32_t queue_index = 0U; queue_index < queue_family_property.queueCount; ++queue_index)
			{
				queues[queue_family_index].emplace_back(*this, queue_family_index, queue_family_property, present_supported, queue_index);
			}
		}
	}

	VkDevice VulkanDevice::getLogicalDevice() const
	{
		return _LogicalDevice;
	}

	const VulkanPhysicalDevice& VulkanDevice::get_gpu() const
	{
		return _gpu;
	}

	const VulkanQueue& VulkanDevice::get_suitable_graphics_queue() const
	{
		for (uint32_t queue_family_index = 0U; queue_family_index < queues.size(); ++queue_family_index)
		{
			VulkanQueue const& first_queue = queues[queue_family_index][0];

			uint32_t queue_count = first_queue.get_properties().queueCount;

			if (first_queue.support_present() && 0 < queue_count)
			{
				return queues[queue_family_index][0];
			}
		}

		return get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);
	}

	const VulkanQueue& VulkanDevice::get_queue_by_present(uint32_t queue_index) const
	{
		for (uint32_t queue_family_index = 0U; queue_family_index < queues.size(); ++queue_family_index)
		{
			VulkanQueue const& first_queue = queues[queue_family_index][0];

			uint32_t queue_count = first_queue.get_properties().queueCount;

			if (first_queue.support_present() && queue_index < queue_count)
			{
				return queues[queue_family_index][queue_index];
			}
		}

		throw std::runtime_error("Queue not found");
	}

	VkCommandPool VulkanDevice::get_command_pool() const
	{
		return _CommandPool;
	}

	VkCommandPool VulkanDevice::createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags)
	{
		VkCommandPoolCreateInfo cmdPoolInfo = {};
		cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
		cmdPoolInfo.flags = createFlags;
		VkCommandPool cmdPool;
		VK_CHECK_RESULT(vkCreateCommandPool(_LogicalDevice, &cmdPoolInfo, nullptr, &cmdPool));
		return cmdPool;
	}
}