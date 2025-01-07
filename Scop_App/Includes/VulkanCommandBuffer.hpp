#pragma once
#include "../Includes/VulkanDevice.hpp"
#include <vector>

namespace vks
{
	class VulkanCommandBuffer
	{
	public:
		VulkanCommandBuffer(VulkanDevice& device, VkCommandPool commandPool, uint32_t bufferCount);
		~VulkanCommandBuffer();

		void begin(VkCommandBuffer commandBuffer);
		void begin_renderpass(
			VkCommandBuffer commandBuffer,
			VkFramebuffer framebuffer,
			VkRenderPass renderpass,
			uint32_t width,
			uint32_t height,
			std::vector<VkClearValue>& clearValues
		);
		void set_viewport(VkCommandBuffer commandBuffer, uint32_t width, uint32_t height);
		void set_scissor(VkCommandBuffer commandBuffer, uint32_t width, uint32_t height);
		void end_renderpass(VkCommandBuffer commandBuffer);
		void end(VkCommandBuffer commandBuffer);

		VkCommandBuffer get_commandBuffer_by_index(uint32_t index) const {
			return _cmdBuffers[index];
		};

		uint32_t get_commandBuffer_size() const {
			return _cmdBuffers.size();
		}


	private:
		VulkanDevice& _vulkanDevice;
		VkCommandPool _CommandPool;
		std::vector<VkCommandBuffer> _cmdBuffers;
	};



}


