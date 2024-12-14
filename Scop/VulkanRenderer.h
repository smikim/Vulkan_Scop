#pragma once
#include "VulkanInstance.h"

namespace vks
{
	class VulkanRenderer
	{
	public:
		VulkanRenderer() {}
		~VulkanRenderer();

		bool initVulkan();

	private:
		vks::VulkanInstance _instance;
	};
}