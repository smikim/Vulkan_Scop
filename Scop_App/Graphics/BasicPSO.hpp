#pragma once

#include <vulkan/vulkan.h>
#include "../Includes/VulkanDevice.hpp"
#include "../Includes/VulkanInitializers.hpp"
#include "../Includes/VulkanTools.hpp"
#include "../Includes/VulkanPipeline.hpp"
#include "../Includes/VulkanPipelineState.hpp"
#include "../Includes/VulkanUtils.hpp"

namespace Graphics
{
	class BasicPSO
	{
	public:
		BasicPSO(vks::VulkanDevice& vulkanDevice, VkRenderPass renderPass);
		~BasicPSO();

		vks::VulkanDevice& _vulkanDevice;

		VkShaderModule _VertShaderModule = VK_NULL_HANDLE;
		VkShaderModule _FragShaderModule = VK_NULL_HANDLE;

		vks::VulkanPipelineState _pipelineState;
		VkPipelineLayout _PipelineLayout{ VK_NULL_HANDLE };

		VkRenderPass _RenderPass;
		void initPSO();
		void setPipelineLayout(VkPipelineLayout pipelineLayout);
	};
}

