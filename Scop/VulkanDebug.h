#pragma once
#include "vulkan/vulkan.h"

namespace vks
{
	namespace debug
	{
		// Default debug callback
		VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessageCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);
		// Used to populate a VkDebugUtilsMessengerCreateInfoEXT with our example messenger function and desired flags
		void setupDebugingMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& debugUtilsMessengerCI);
		// Clear debug callback
		void freeDebugCallback(VkInstance instance);
		void setupDebugging(VkInstance instance);
	}


}

