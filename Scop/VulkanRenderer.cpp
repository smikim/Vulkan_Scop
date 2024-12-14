#include "VulkanRenderer.h"
#include "VulkanTools.h"

namespace vks
{
	vks::VulkanRenderer::~VulkanRenderer()
	{

	}

	bool VulkanRenderer::initVulkan()
	{
		VK_CHECK_RESULT(_instance.createInstance());

		return true;
	}

}

