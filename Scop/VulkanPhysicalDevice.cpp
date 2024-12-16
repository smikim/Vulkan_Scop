#include "VulkanPhysicalDevice.h"

namespace vks
{
	VulkanPhysicalDevice::VulkanPhysicalDevice(VulkanInstance& instance, VkPhysicalDevice physical_device)
		: _Instance(instance), _PhysicalDevice(physical_device)
	{
		vkGetPhysicalDeviceFeatures(_PhysicalDevice, &_DeviceFeatures);
		vkGetPhysicalDeviceProperties(_PhysicalDevice, &_DeviceProperties);
		vkGetPhysicalDeviceMemoryProperties(_PhysicalDevice, &_DeviceMemoryProperties);


		// TODO
		if (_DeviceFeatures.samplerAnisotropy) {
			//requested_features.samplerAnisotropy = VK_TRUE;
		};


		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(_PhysicalDevice, &queueFamilyCount, nullptr);
		assert(queueFamilyCount > 0);
		_queueFamilyProperties.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(_PhysicalDevice, &queueFamilyCount, _queueFamilyProperties.data());

		// Get list of supported extensions
		uint32_t extCount = 0;
		vkEnumerateDeviceExtensionProperties(_PhysicalDevice, nullptr, &extCount, nullptr);
		if (extCount > 0)
		{
			std::vector<VkExtensionProperties> extensions(extCount);
			if (vkEnumerateDeviceExtensionProperties(_PhysicalDevice, nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
			{
				for (auto& ext : extensions)
				{
					_supportedExtensions.push_back(ext.extensionName);
				}
			}
		}

	}
	const VkPhysicalDeviceFeatures VulkanPhysicalDevice::get_device_features() const
	{
		return _DeviceFeatures;
	}

	const VkPhysicalDeviceProperties& VulkanPhysicalDevice::get_device_properties() const
	{
		return _DeviceProperties;
	}

	const std::vector<VkQueueFamilyProperties>& VulkanPhysicalDevice::get_queue_family_properties() const
	{
		return _queueFamilyProperties;
	}

	bool VulkanPhysicalDevice::is_extension_supported(const std::string& requested_extension) const
	{
		return std::find_if(_supportedExtensions.begin(), _supportedExtensions.end(),
			[requested_extension](auto& device_extension) {
				return std::strcmp(device_extension.c_str(), requested_extension.c_str()) == 0;
			}) != _supportedExtensions.end();
	}

	void* VulkanPhysicalDevice::get_extension_feature_chain() const
	{
		return _last_requested_extension_feature;
	}

	const VkPhysicalDeviceFeatures VulkanPhysicalDevice::get_requested_features() const
	{
		return _requested_features;
	}

	const VkPhysicalDevice VulkanPhysicalDevice::get_physical_device() const
	{
		return _PhysicalDevice;
	}

	VkBool32 VulkanPhysicalDevice::is_present_supported(VkSurfaceKHR surface, uint32_t queue_family_index) const
	{
		VkBool32 present_supported{ VK_FALSE };

		if (surface != VK_NULL_HANDLE)
		{
			VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(_PhysicalDevice, queue_family_index, surface, &present_supported));
		}

		return present_supported;
	}
}