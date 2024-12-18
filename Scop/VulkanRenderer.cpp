#include "VulkanRenderer.h"
#include "VulkanTools.h"

namespace vks
{
	VulkanRenderer::VulkanRenderer(GlfwWindow& window)
		: _window(window)
	{
		add_device_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	}

	vks::VulkanRenderer::~VulkanRenderer()
	{
		vkDestroyPipelineLayout(_vulkanDevice->getLogicalDevice(), _basicPipelineLayout, nullptr);
		_swapChain->cleanup();
		
		VkDevice logicalDevice = _vulkanDevice->getLogicalDevice();

		vkDestroyImageView(logicalDevice, _depthStencil.view, nullptr);
		vkDestroyImage(logicalDevice, _depthStencil.image, nullptr);
		vkFreeMemory(logicalDevice, _depthStencil.memory, nullptr);

		if (_RenderPass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(logicalDevice, _RenderPass, nullptr);
		}

		for (uint32_t i = 0; i < _FrameBuffers.size(); i++)
		{
			vkDestroyFramebuffer(logicalDevice, _FrameBuffers[i], nullptr);
		}
		delete _vulkanDevice;

		
	}

	bool VulkanRenderer::initVulkan()
	{
		VK_CHECK_RESULT(_instance.createInstance());
		create_surface();
		_vulkanDevice = new vks::VulkanDevice(_instance.get_first_gpu(), _Surface, get_device_extensions());
		_swapChain = std::make_unique<VulkanSwapChain>(_instance, *_vulkanDevice, _Surface);
		_swapChain->initSurface(_window.getGLFWwindow());
		_width = _window.getWidth();
		_height = _window.getHeight();
		_swapChain->create(_width, _height);
		// Find a suitable depth and/or stencil format
		VkBool32 validFormat{ false };
		VkPhysicalDevice physicalDevice = _vulkanDevice->get_gpu().get_physical_device();
		// Samples that make use of stencil will require a depth + stencil format, so we select from a different list
		if (_requiresStencil) {
			validFormat = vks::tools::getSupportedDepthStencilFormat(physicalDevice, &_DepthFormat);
		}
		else {
			validFormat = vks::tools::getSupportedDepthFormat(physicalDevice, &_DepthFormat);
		}
		assert(validFormat); 
		setupDepthStencil(_width, _height);
		setupRenderPass();
		setupFrameBuffer(_width, _height);

		_basicPSO = new Graphics::BasicPSO(*_vulkanDevice, _RenderPass);
		createPipelineLayout();
		init_basicPipeline(_basicPSO, _basicPipelineLayout);


		return true;
	}

	void VulkanRenderer::init_basicPipeline(Graphics::BasicPSO* basicPSO, VkPipelineLayout pipelineLayout)
	{
		basicPSO->setPipelineLayout(pipelineLayout);
		basicPSO->initPSO();

		// Scene rendering with shadows applied
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

		VkPipelineShaderStageCreateInfo shaderStage{};
		shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

		shaderStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStage.module = basicPSO->_VertShaderModule;
		shaderStage.pName = "main";

		shaderStage.pNext = nullptr;
		shaderStage.pSpecializationInfo = nullptr;

		shaderStages.push_back(shaderStage);

		shaderStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStage.module = basicPSO->_FragShaderModule;

		shaderStages.push_back(shaderStage);

		_basicPipeline = new VulkanPipeline(*_vulkanDevice, shaderStages, basicPSO->_pipelineState);
	}

	void VulkanRenderer::getEnabledFeatures()
	{

	}

	void VulkanRenderer::create_surface()
	{
		if (glfwCreateWindowSurface(_instance.getInstance(), _window.getGLFWwindow(), nullptr, &_Surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface");
		}
	}

	void VulkanRenderer::createPipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutCI{};
		pipelineLayoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCI.pNext = nullptr;
		pipelineLayoutCI.setLayoutCount = 0;
		pipelineLayoutCI.pSetLayouts = nullptr;
		pipelineLayoutCI.pushConstantRangeCount = 0;
		pipelineLayoutCI.pPushConstantRanges = nullptr;

		VK_CHECK_RESULT(vkCreatePipelineLayout(_vulkanDevice->getLogicalDevice(), &pipelineLayoutCI, nullptr, &_basicPipelineLayout));

	}

	void VulkanRenderer::setupRenderPass()
	{
		std::array<VkAttachmentDescription, 2> attachments = {};
		// Color attachment
		attachments[0].format = _swapChain->_ColorFormat;
		attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		// Depth attachment
		attachments[1].format = _DepthFormat;
		attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorReference = {};
		colorReference.attachment = 0;
		colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthReference = {};
		depthReference.attachment = 1;
		depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorReference;
		subpassDescription.pDepthStencilAttachment = &depthReference;
		subpassDescription.inputAttachmentCount = 0;
		subpassDescription.pInputAttachments = nullptr;
		subpassDescription.preserveAttachmentCount = 0;
		subpassDescription.pPreserveAttachments = nullptr;
		subpassDescription.pResolveAttachments = nullptr;

		// Subpass dependencies for layout transitions
		std::array<VkSubpassDependency, 2> dependencies;

		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
		dependencies[0].dependencyFlags = 0;

		dependencies[1].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[1].dstSubpass = 0;
		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[1].srcAccessMask = 0;
		dependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		dependencies[1].dependencyFlags = 0;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpassDescription;
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		VkDevice logicalDevice = _vulkanDevice->getLogicalDevice();
		VK_CHECK_RESULT(vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &_RenderPass));
	}

	void VulkanRenderer::setupDepthStencil(uint32_t& width, uint32_t& height)
	{
		VkImageCreateInfo imageCI{};
		imageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCI.imageType = VK_IMAGE_TYPE_2D;
		imageCI.format = _DepthFormat;
		imageCI.extent = { width, height, 1 };
		imageCI.mipLevels = 1;
		imageCI.arrayLayers = 1;
		imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

		VkDevice logicalDevice = _vulkanDevice->getLogicalDevice();

		VK_CHECK_RESULT(vkCreateImage(logicalDevice, &imageCI, nullptr, &_depthStencil.image));
		VkMemoryRequirements memReqs{};
		vkGetImageMemoryRequirements(logicalDevice, _depthStencil.image, &memReqs);


		VkMemoryAllocateInfo memAllloc{};
		memAllloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memAllloc.allocationSize = memReqs.size;
		memAllloc.memoryTypeIndex = _vulkanDevice->get_gpu().getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(logicalDevice, &memAllloc, nullptr, &_depthStencil.memory));
		VK_CHECK_RESULT(vkBindImageMemory(logicalDevice, _depthStencil.image, _depthStencil.memory, 0));

		VkImageViewCreateInfo imageViewCI{};
		imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCI.image = _depthStencil.image;
		imageViewCI.format = _DepthFormat;
		imageViewCI.subresourceRange.baseMipLevel = 0;
		imageViewCI.subresourceRange.levelCount = 1;
		imageViewCI.subresourceRange.baseArrayLayer = 0;
		imageViewCI.subresourceRange.layerCount = 1;
		imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		// Stencil aspect should only be set on depth + stencil formats (VK_FORMAT_D16_UNORM_S8_UINT..VK_FORMAT_D32_SFLOAT_S8_UINT
		if (_DepthFormat >= VK_FORMAT_D16_UNORM_S8_UINT) {
			imageViewCI.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
		VK_CHECK_RESULT(vkCreateImageView(logicalDevice, &imageViewCI, nullptr, &_depthStencil.view));

	}

	void VulkanRenderer::setupFrameBuffer(uint32_t& width, uint32_t& height)
	{
		VkImageView attachments[2];

		// Depth/Stencil attachment is the same for all frame buffers
		attachments[1] = _depthStencil.view;

		VkFramebufferCreateInfo frameBufferCreateInfo = {};
		frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferCreateInfo.pNext = NULL;
		frameBufferCreateInfo.renderPass = _RenderPass;
		frameBufferCreateInfo.attachmentCount = 2;
		frameBufferCreateInfo.pAttachments = attachments;
		frameBufferCreateInfo.width = width;
		frameBufferCreateInfo.height = height;
		frameBufferCreateInfo.layers = 1;

		VkDevice logicalDevice = _vulkanDevice->getLogicalDevice();

		// Create frame buffers for every swap chain image
		_FrameBuffers.resize(_swapChain->_imageCount);
		for (uint32_t i = 0; i < _FrameBuffers.size(); i++)
		{
			attachments[0] = _swapChain->_buffers[i].view;
			VK_CHECK_RESULT(vkCreateFramebuffer(logicalDevice, &frameBufferCreateInfo, nullptr, &_FrameBuffers[i]));
		}
	}

}

