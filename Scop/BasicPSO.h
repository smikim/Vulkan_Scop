#pragma once

#include <vulkan/vulkan.h>
#include "VulkanDevice.h"
#include "VulkanInitializers.h"
#include "VulkanTools.h"
#include "VulkanPipeline.h"
#include "VulkanPipelineState.h"
#include "VulkanUtils.h"

namespace Graphics
{
	class BasicPSO
	{
	public:
		BasicPSO(vks::VulkanDevice& vulkanDevice, VkRenderPass renderPass);
		~BasicPSO();

		vks::VulkanDevice& _vulkanDevice;

		VkShaderModule _VertShaderModule;
		VkShaderModule _FragShaderModule;

		vks::VulkanPipelineState _pipelineState;
		VkPipelineLayout _PipelineLayout{ VK_NULL_HANDLE };

		VkRenderPass _RenderPass;
		void initPSO();
		void setPipelineLayout(VkPipelineLayout pipelineLayout);
	};
}

