#include "../Includes/VulkanCommandBuffer.hpp"
#include "../Includes/VulkanTools.hpp"
#include "../Includes/VulkanInitializers.hpp"


namespace vks
{
	VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice& device, VkCommandPool commandPool, uint32_t bufferCount)
		: _vulkanDevice(device), _CommandPool(commandPool)
	{
		VkDevice logicalDevice = _vulkanDevice.getLogicalDevice();

		// Allocate one command buffer per max. concurrent frame from above pool
		VkCommandBufferAllocateInfo cmdBufAllocateInfo = vks::initializers::commandBufferAllocateInfo(_CommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, bufferCount);
		_cmdBuffers.resize(bufferCount);
		VK_CHECK_RESULT(vkAllocateCommandBuffers(logicalDevice, &cmdBufAllocateInfo, _cmdBuffers.data()));
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		VkDevice logicalDevice = _vulkanDevice.getLogicalDevice();
		if (_cmdBuffers.size() > 0)
		{
			vkFreeCommandBuffers(
			logicalDevice,
			_CommandPool,
			static_cast<uint32_t>(_cmdBuffers.size()),
			_cmdBuffers.data());
		}
		
	}

	void VulkanCommandBuffer::begin(VkCommandBuffer commandBuffer)
	{
		VkCommandBufferBeginInfo cmdBufInfo{};
		cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &cmdBufInfo));
	}

	void VulkanCommandBuffer::begin_renderpass(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer,
		VkRenderPass renderpass, uint32_t width, uint32_t height, std::vector<VkClearValue>& clearValues)
	{
		//std::cout << renderpass << std::endl;

		/*VkClearValue clearValuesT[2];
		clearValues[0].color = { { 1.f, 0.f, 0.f, 1.0f } };;
		clearValues[1].depthStencil = { 1.0f, 0 };*/

		VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
		renderPassBeginInfo.renderPass = renderpass;
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = width;
		renderPassBeginInfo.renderArea.extent.height = height;
		renderPassBeginInfo.clearValueCount = clearValues.size();
		renderPassBeginInfo.pClearValues = clearValues.data();
		renderPassBeginInfo.framebuffer = framebuffer;

		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanCommandBuffer::set_viewport(VkCommandBuffer commandBuffer, uint32_t width, uint32_t height)
	{
		const VkViewport viewport = vks::initializers::viewport((float)width, (float)height, 0.0f, 1.0f);
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	}

	void VulkanCommandBuffer::set_scissor(VkCommandBuffer commandBuffer, uint32_t width, uint32_t height)
	{
		const VkRect2D scissor = vks::initializers::rect2D(width, height, 0, 0);

		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void VulkanCommandBuffer::end_renderpass(VkCommandBuffer commandBuffer)
	{
		vkCmdEndRenderPass(commandBuffer);

	}

	void VulkanCommandBuffer::end(VkCommandBuffer commandBuffer)
	{
		VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));
	}


}


