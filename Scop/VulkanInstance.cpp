#include "VulkanInstance.h"
#include <iostream>

namespace vks
{
	VulkanInstance::~VulkanInstance()
	{
		if (enableValidationLayers)
		{
			vks::debug::freeDebugCallback(_Instance);
		}

		vkDestroyInstance(_Instance, nullptr);
	}

	VkResult VulkanInstance::createInstance()
	{
		for (const char* extension_name : get_required_surface_extensions())
		{
			_instanceExtensions.push_back(extension_name);
		}

		// Get extensions supported by the instance and store for later use
		uint32_t extCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
		if (extCount > 0)
		{
			std::vector<VkExtensionProperties> extensions(extCount);
			if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
			{
				std::cout << "available extensions:\n";
				for (VkExtensionProperties& extension : extensions)
				{
					std::cout << '\t' << extension.extensionName << '\n';
					_supportedInstanceExtensions.push_back(extension.extensionName);
				}
			}
		}

		// Enabled requested instance extensions
		if (_enabledInstanceExtensions.size() > 0)
		{
			for (const char* enabledExtension : _enabledInstanceExtensions)
			{
				// Output message if requested extension is not available
				if (std::find(_supportedInstanceExtensions.begin(), _supportedInstanceExtensions.end(), enabledExtension) == _supportedInstanceExtensions.end())
				{
					std::cerr << "Enabled instance extension \"" << enabledExtension << "\" is not present at instance level\n";
				}
				_instanceExtensions.push_back(enabledExtension);
			}
		}
		

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = name.c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = name.c_str();
		appInfo.apiVersion = apiVersion;
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

		VkInstanceCreateInfo instanceCreateInfo{};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &appInfo;

		VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCI{};
		if (enableValidationLayers) {
			vks::debug::setupDebugingMessengerCreateInfo(debugUtilsMessengerCI);
			debugUtilsMessengerCI.pNext = instanceCreateInfo.pNext;
			instanceCreateInfo.pNext = &debugUtilsMessengerCI;
		}

		// Enable the debug utils extension if available (e.g. when debugging tools are present)
		if (enableValidationLayers || std::find(_supportedInstanceExtensions.begin(), _supportedInstanceExtensions.end(), VK_EXT_DEBUG_UTILS_EXTENSION_NAME) != _supportedInstanceExtensions.end()) {
			_instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		if (_instanceExtensions.size() > 0) {
			instanceCreateInfo.enabledExtensionCount = (uint32_t)_instanceExtensions.size();
			instanceCreateInfo.ppEnabledExtensionNames = _instanceExtensions.data();
		}

		//instanceCreateInfo.enabledLayerCount = 0;

		// The VK_LAYER_KHRONOS_validation contains all current validation functionality.
		// Note that on Android this layer requires at least NDK r20
		const char* validationLayerName = "VK_LAYER_KHRONOS_validation";

		if (enableValidationLayers) {
			// Check if this layer is available at instance level
			uint32_t instanceLayerCount;
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
			std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
			bool validationLayerPresent = false;
			for (VkLayerProperties& layer : instanceLayerProperties) {
				if (strcmp(layer.layerName, validationLayerName) == 0) {
					validationLayerPresent = true;
					break;
				}
			}
			if (validationLayerPresent) {
				instanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
				instanceCreateInfo.enabledLayerCount = 1;
			}
			else {
				std::cerr << "Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled";
			}
		}


		VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &_Instance);

		// If requested, we enable the default validation layers for debugging
		if (enableValidationLayers)
		{
			vks::debug::setupDebugging(_Instance);
		}

		query_gpus();

		return result;
	}

	VkInstance VulkanInstance::getInstance() const
	{
		return _Instance;
	}

	VulkanPhysicalDevice& VulkanInstance::get_first_gpu()
	{
		assert(!gpus.empty() && "No physical devices were found on the system.");

		// Find a discrete GPU
		for (auto& gpu : gpus)
		{
			if (gpu->get_device_properties().deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				return *gpu;
			}
		}

		// Otherwise just pick the first one
		std::cerr << "Couldn't find a discrete physical device, picking default GPU" << std::endl;
		return *gpus[0];
	}

	std::vector<const char*> VulkanInstance::get_required_surface_extensions() const
	{
		uint32_t     glfw_extension_count{ 0 };
		const char** names = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

		std::vector<const char*> extensions{ names, names + glfw_extension_count };
		/*if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}*/

		return extensions;
	}

	void VulkanInstance::query_gpus()
	{
		// Physical device
		uint32_t gpuCount = 0;
		// Get number of available physical devices
		VK_CHECK_RESULT(vkEnumeratePhysicalDevices(_Instance, &gpuCount, nullptr));
		if (gpuCount == 0) {
			throw std::runtime_error("No device with Vulkan support found");
		}
		// Enumerate devices
		std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
		VkResult result = vkEnumeratePhysicalDevices(_Instance, &gpuCount, physicalDevices.data());
		if (result != VK_SUCCESS) {

			throw std::runtime_error("Could not enumerate physical devices : \n");
		}

		// Create gpus wrapper objects from the VkPhysicalDevice's
		for (auto& physical_device : physicalDevices)
		{
			gpus.push_back(std::make_unique<VulkanPhysicalDevice>(*this, physical_device));
		}
	}
}