#include "VulkanRenderer.h"
#include "VulkanTools.h"
#include "Scop.h"

#include <glm/gtc/type_ptr.hpp>
namespace vks
{
	VulkanRenderer::VulkanRenderer(GlfwWindow& window)
		: _window(window)
	{
		add_device_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	}

	vks::VulkanRenderer::~VulkanRenderer()
	{
		vkDestroyDescriptorPool(_vulkanDevice->getLogicalDevice(), _basicDescriptorPool, nullptr);
		vkDestroyDescriptorSetLayout(_vulkanDevice->getLogicalDevice(), _basicDescriptorSetLayout, nullptr);
		
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

		for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES; i++) {
			vkDestroyFence(logicalDevice, _WaitFences[i], nullptr);
			vkDestroySemaphore(logicalDevice, _PresentCompleteSemaphores[i], nullptr);
			vkDestroySemaphore(logicalDevice, _RenderCompleteSemaphores[i], nullptr);
			vkDestroyBuffer(logicalDevice, _uniformBuffers[i].buffer, nullptr);
			vkFreeMemory(logicalDevice, _uniformBuffers[i].memory, nullptr);
		}

		delete _drawCommandBuffer;

		delete _basicPSO;
		delete _basicPipeline;

		vkDestroyCommandPool(_vulkanDevice->getLogicalDevice(), _CmdPool, nullptr);

		delete _texture;
		//delete _model;
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
		createSynchronizationPrimitives();
		createCommandPool();

		

		_drawCommandBuffer = new VulkanCommandBuffer(*_vulkanDevice, _CmdPool, _swapChain->_imageCount);

		_basicPSO = new Graphics::BasicPSO(*_vulkanDevice, _RenderPass);
		
		const VulkanQueue& queue = _vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);

		// TODO
		// loadTexture();

		std::string filename{ "textures/lena.bmp" };
		
		//std::string filename{ "textures/sample.bmp" };
		
		//std::string filename{ "textures/640-480-sample.bmp" };
		//std::string filename{ "textures/blackbuck.bmp" };
		//std::string filename{ "textures/bmp_24.bmp" };
		
		//std::string filename{ "textures/dots.bmp" };

		_texture = new VulkanTexture(filename, queue.get_queue(), _vulkanDevice);

		createUniformBuffers();
		createDescriptorSetLayout();
		createDescriptorPool();
		createDescriptorSets();

		createPipelineLayout();

		

		init_basicPipeline(_basicPSO, _basicPipelineLayout);

		_prepared = true;

		// TODO 
		
		//_model = new VulkanModel(*_vulkanDevice);

		//_model->createVertexBuffer(queue.get_queue());

		_camera.setPerspectiveProjection(glm::radians(45.0f), getAspectRatio(), 0.1f, 10.0f);
		_camera.setViewTarget(glm::vec3(0.0f, 0.0f, -8.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		
		glm::mat4 cameraView = _camera.getView();
		std::cout << "---- cameraView -----" << std::endl;
		const float* pSource1 = (const float*)glm::value_ptr(cameraView);
		for (int i = 0; i < 16; ++i) {
			std::cout << pSource1[i] << " ";
			if ((i + 1) % 4 == 0) std::cout << std::endl;
		}
		std::cout << "---- cameraPer -----" << std::endl;
		glm::mat4 cameraPer = _camera.getProjection();
		pSource1 = (const float*)glm::value_ptr(cameraPer);
		for (int i = 0; i < 16; ++i) {
			std::cout << pSource1[i] << " ";
			if ((i + 1) % 4 == 0) std::cout << std::endl;
		}
		return true;
	}

	VulkanModel* VulkanRenderer::CreateBasicMeshObject()
	{
		VulkanModel* vulkanModel = new VulkanModel;
		vulkanModel->Initialize(this);

		return vulkanModel;
	}

	void VulkanRenderer::BeginCreateMesh(VulkanModel* model, std::vector<vks::VulkanModel::Vertex>& vertices)
	{
		model->createVertexBuffer(vertices);

	}

	void VulkanRenderer::InsertIndexBuffer(VulkanModel* model, std::vector<uint32_t>& indices)
	{
		model->createIndexBuffer(indices);
	}

	void VulkanRenderer::EndCreateMesh(VulkanModel* model)
	{
		model->EndCreateMesh();
	}

	void VulkanRenderer::DeleteMeshObject(VulkanModel* model)
	{
		delete model;
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

	VkResult VulkanRenderer::beginRender()
	{
		auto result = _swapChain->acquireNextImage(_WaitFences[_currentFrame], _PresentCompleteSemaphores[_currentFrame], &_currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			_window.resetWindowResizedFlag();
			windowResize();
			std::cout << "beginFrame(): return nullptr" << std::endl;
			return result;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			//throw std::runtime_error("failed to acquire swap chain image!");
			return result;
		}

		VkCommandBuffer commandBuffer = getCurrentCommandBuffer();
		
		// Build the command buffer
		// Unlike in OpenGL all rendering commands are recorded into command buffers that are then submitted to the queue
		// This allows to generate work upfront in a separate thread
		// For basic command buffers (like in this sample), recording is so fast that there is no need to offload this

		vkResetCommandBuffer(commandBuffer, 0);
		//std::cout << commandBuffer << std::endl;

		_drawCommandBuffer->begin(commandBuffer);

		return VK_SUCCESS;
	}

	void VulkanRenderer::beginRenderPass()
	{
		std::vector<VkClearValue> clearValues;
		VkClearValue clearValue;

		clearValue.color = _defaultClearColor;
		clearValues.push_back(clearValue);

		clearValue.depthStencil = { 1.0f, 0 };
		clearValues.push_back(clearValue);
	



		//std::vector<VkClearValue> clearValues(2);

		////clearValues[0].color = _defaultClearColor;
		//clearValues[0].color = { { 1.0f, 0.0f, 0.0f, 1.0f } };
		//clearValues[1].depthStencil = { 1.0f, 0 };


		uint32_t width = getWidth();
		uint32_t height = getHeight();
		
		VkCommandBuffer commandBuffer = getCurrentCommandBuffer();

		_drawCommandBuffer->begin_renderpass(commandBuffer, get_frameBuffer_by_index(_currentImageIndex), _RenderPass, width, height, clearValues);

		_drawCommandBuffer->set_viewport(commandBuffer, width, height);
		_drawCommandBuffer->set_scissor(commandBuffer, width, height);

	}

	void VulkanRenderer::endRenderPass()
	{
		VkCommandBuffer commandBuffer = getCurrentCommandBuffer();

		_drawCommandBuffer->end_renderpass(commandBuffer);

		_drawCommandBuffer->end(commandBuffer);
	}

	VkResult VulkanRenderer::endRender()
	{
		auto result = submitCommandBuffer();

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _window.wasWindowResized()) {
			if (_window.wasWindowResized()) {
				result = VK_ERROR_OUT_OF_DATE_KHR;
			}
			_window.resetWindowResizedFlag();
			windowResize();
			return result;
		}
		else if (result != VK_SUCCESS) {
			return result;
			//throw std::runtime_error("failed to present swap chain image!");
		}

		return VK_SUCCESS;
	}

	void VulkanRenderer::renderMeshObject(VulkanModel* object)
	{
		VkCommandBuffer commandBuffer = getCurrentCommandBuffer();

		if (object)
		{
			// render object

			_basicPipeline->bind(commandBuffer);

			// Bind descriptor set for the currrent frame's uniform buffer, so the shader uses the data from that buffer for this draw

			// TODO 
			// renderGameObject 같은 함수를 만들어서 
			// VulkanModel에서 가지고 있는 자기의 descriptorSets를 바인딩 시킨후 , 
			// vertex, index buffer를 바인딩 시킨후
			// draw 해주는 방향으로 수정 해야함 !!

			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _basicPipelineLayout, 0, 1, &_uniformBuffers[_currentFrame].descriptorSet, 0, nullptr);

			object->bind(commandBuffer);
			object->draw(commandBuffer);
		}
	}

	void VulkanRenderer::buildBasicCommandBuffers()
	{
		if (!_prepared)
			return;

		//vks::VulkanCommandBuffer * drawcommandBuffers = getDrawCommandBuffers();

		// TODO
		uint32_t index = 0;

		for (int32_t i = 0; i < _drawCommandBuffer->get_commandBuffer_size(); ++i)
		{
			VkCommandBuffer commandBuffer = _drawCommandBuffer->get_commandBuffer_by_index(i);

			//std::cout << commandBuffer << std::endl;

			std::vector<VkClearValue> clearValues;
			VkClearValue clearValue;

			clearValue.color = _defaultClearColor;
			clearValues.push_back(clearValue);

			clearValue.depthStencil = { 1.0f, 0 };
			clearValues.push_back(clearValue);

			uint32_t width = getWidth();
			uint32_t height = getHeight();

			_drawCommandBuffer->begin(commandBuffer);

			_drawCommandBuffer->begin_renderpass(commandBuffer, get_frameBuffer_by_index(i), _RenderPass, width, height, clearValues);


			_drawCommandBuffer->set_viewport(commandBuffer, width, height);
			_drawCommandBuffer->set_scissor(commandBuffer, width, height);

			// render object

			_basicPipeline->bind(commandBuffer);

			index = (index + 1) % MAX_CONCURRENT_FRAMES;
			// Bind descriptor set for the currrent frame's uniform buffer, so the shader uses the data from that buffer for this draw
			
			// TODO 
			// renderGameObject 같은 함수를 만들어서 
			// VulkanModel에서 가지고 있는 자기의 descriptorSets를 바인딩 시킨후 , 
			// vertex, index buffer를 바인딩 시킨후
			// draw 해주는 방향으로 수정 해야함 !!

			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _basicPipelineLayout, 0, 1, &_uniformBuffers[index].descriptorSet, 0, nullptr);

			_model->bind(commandBuffer);
			_model->draw(commandBuffer);


			//vkCmdDraw(commandBuffer, 3, 1, 0, 0);

			//VkDeviceSize offsets[1] = { 0 };
			//vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer.buffer, offsets);
			//vkCmdBindIndexBuffer(commandBuffer, indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);

			////uint32_t dynamicOffset = static_cast<uint32_t>(dynamicUniformInfo.dynamicAlignment);
			//uint32_t dynamicOffset = 0;
			//// Bind the descriptor set for rendering a mesh using the dynamic offset
			//vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _basicPipelineLayout, 0, 1, &dynamicUniformInfo.descriptorSet, 1, &dynamicOffset);

			//vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);

			//renderGameObjects(commandBuffer, ObjgameObjects, dynamicUniformInfo.descriptorSet);

			_drawCommandBuffer->end_renderpass(commandBuffer);

			_drawCommandBuffer->end(commandBuffer);

		}
	}

	VkResult VulkanRenderer::prepareFrame()
	{
		auto result = _swapChain->acquireNextImage(_WaitFences[_currentFrame], _PresentCompleteSemaphores[_currentFrame], &_currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			_window.resetWindowResizedFlag();
			windowResize();
			std::cout << "beginFrame(): return nullptr" << std::endl;
			return result;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			//throw std::runtime_error("failed to acquire swap chain image!");
			return result;
		}

		return VK_SUCCESS;
	}

	VkResult VulkanRenderer::submitFrame()
	{
		auto result = submitCommandBuffer();

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _window.wasWindowResized()) {
			if (_window.wasWindowResized()) {
				result = VK_ERROR_OUT_OF_DATE_KHR;
			}
			_window.resetWindowResizedFlag();
			windowResize();
			return result;
		}
		else if (result != VK_SUCCESS) {
			return result;
			//throw std::runtime_error("failed to present swap chain image!");
		}
		return VK_SUCCESS;
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
		pipelineLayoutCI.setLayoutCount = 1;
		pipelineLayoutCI.pSetLayouts = &_basicDescriptorSetLayout;
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

	void VulkanRenderer::createCommandPool()
	{
		VkCommandPoolCreateInfo cmdPoolInfo = {};
		cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolInfo.queueFamilyIndex = _swapChain->_queueNodeIndex;
		cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		VK_CHECK_RESULT(vkCreateCommandPool(_vulkanDevice->getLogicalDevice(), &cmdPoolInfo, nullptr, &_CmdPool));
	}


	void VulkanRenderer::createSynchronizationPrimitives()
	{
		// Semaphores are used for correct command ordering within a queue
		VkSemaphoreCreateInfo semaphoreCI{};
		semaphoreCI.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		// Fences are used to check draw command buffer completion on the host
		VkFenceCreateInfo fenceCI{};
		fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		// Create the fences in signaled state (so we don't wait on first render of each command buffer)
		fenceCI.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		VkDevice logicalDevice = _vulkanDevice->getLogicalDevice();

		for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES; i++) {
			// Semaphore used to ensure that image presentation is complete before starting to submit again
			VK_CHECK_RESULT(vkCreateSemaphore(logicalDevice, &semaphoreCI, nullptr, &_PresentCompleteSemaphores[i]));
			// Semaphore used to ensure that all commands submitted have been finished before submitting the image to the queue
			VK_CHECK_RESULT(vkCreateSemaphore(logicalDevice, &semaphoreCI, nullptr, &_RenderCompleteSemaphores[i]));

			// Fence used to ensure that command buffer has completed exection before using it again
			VK_CHECK_RESULT(vkCreateFence(logicalDevice, &fenceCI, nullptr, &_WaitFences[i]));
		}
	}

	VkResult VulkanRenderer::submitCommandBuffer()
	{
		// Submit the command buffer to the graphics queue

		VkCommandBuffer commandBuffer = getCurrentCommandBuffer();

		// TODO: 코드 정리

		// Pipeline stage at which the queue submission will wait (via pWaitSemaphores)
		VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		// The submit info structure specifies a command buffer queue submission batch
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pWaitDstStageMask = &waitStageMask;      // Pointer to the list of pipeline stages that the semaphore waits will occur at
		submitInfo.pCommandBuffers = &commandBuffer;		// Command buffers(s) to execute in this batch (submission)
		submitInfo.commandBufferCount = 1;                  // We submit a single command buffer

		// Semaphore to wait upon before the submitted command buffer starts executing
		submitInfo.pWaitSemaphores = &_PresentCompleteSemaphores[_currentFrame];
		submitInfo.waitSemaphoreCount = 1;
		// Semaphore to be signaled when command buffers have completed
		submitInfo.pSignalSemaphores = &_RenderCompleteSemaphores[_currentFrame];
		submitInfo.signalSemaphoreCount = 1;


		// Submit to the graphics queue passing a wait fence
		//std::cout << _Queue.get_queue() << std::endl;
		const VulkanQueue& queue = _vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);
		VK_CHECK_RESULT(vkQueueSubmit(queue.get_queue(), 1, &submitInfo, _WaitFences[_currentFrame]));

		VkResult result = _swapChain->queuePresent(queue.get_queue(), _currentImageIndex, _RenderCompleteSemaphores[_currentFrame]);

		_currentFrame = (_currentFrame + 1) % MAX_CONCURRENT_FRAMES;

		return result;

	}

	void VulkanRenderer::windowResize()
	{
		VkDevice logicalDevice = _vulkanDevice->getLogicalDevice();
		std::cout << "windowResize()" << std::endl;
		if (!_prepared)
		{

			return;
		}
		_prepared = false;
		_resized = true;

		vkDeviceWaitIdle(logicalDevice);

		auto extent = _window.getExtent();

		while (extent.width == 0 || extent.height == 0) {
			extent = _window.getExtent();
			glfwWaitEvents();
		}
		
		_width = extent.width;
		_height = extent.height;

		_swapChain->create(_width, _height);
		// Recreate the frame buffers
		vkDestroyImageView(logicalDevice, _depthStencil.view, nullptr);
		vkDestroyImage(logicalDevice, _depthStencil.image, nullptr);
		vkFreeMemory(logicalDevice, _depthStencil.memory, nullptr);
		setupDepthStencil(_width, _height);
		for (uint32_t i = 0; i < _FrameBuffers.size(); i++) {
			vkDestroyFramebuffer(logicalDevice, _FrameBuffers[i], nullptr);
		}
		setupFrameBuffer(_width, _height);

		delete _drawCommandBuffer;
		_drawCommandBuffer = new VulkanCommandBuffer(*_vulkanDevice, _CmdPool, _swapChain->_imageCount);

		// Command buffers need to be recreated as they may store
		// references to the recreated frame buffer
		//destroyCommandBuffers();
		//createCommandBuffers();
		//buildCommandBuffers();


		//for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES; i++) {
		//	vkDestroyFence(logicalDevice, _WaitFences[i], nullptr);
		//	vkDestroySemaphore(logicalDevice, _PresentCompleteSemaphores[i], nullptr);
		//	vkDestroySemaphore(logicalDevice, _RenderCompleteSemaphores[i], nullptr);
		//	//vkDestroyBuffer(device, uniformBuffers[i].buffer, nullptr);
		//	//vkFreeMemory(device, uniformBuffers[i].memory, nullptr);
		//}
		//createSynchronizationPrimitives();

		vkDeviceWaitIdle(logicalDevice);


		_prepared = true;
	}

	void VulkanRenderer::createDescriptorSetLayout()
	{
		std::array<VkDescriptorSetLayoutBinding, 2> setLayoutBindings{};

		// Binding 0: Uniform buffer (Vertex shader)
		setLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		setLayoutBindings[0].binding = 0;
		setLayoutBindings[0].descriptorCount = 1;
		setLayoutBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		setLayoutBindings[0].pImmutableSamplers = nullptr;

		/*
			Binding 1: Combined image sampler (used to pass per object texture information)
		*/
		setLayoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		setLayoutBindings[1].binding = 1;
		// Accessible from the fragment shader only
		setLayoutBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		setLayoutBindings[1].descriptorCount = 1;
		setLayoutBindings[1].pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo descriptorLayoutCI{};
		descriptorLayoutCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorLayoutCI.pNext = nullptr;
		descriptorLayoutCI.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
		descriptorLayoutCI.pBindings = setLayoutBindings.data();
		VK_CHECK_RESULT(vkCreateDescriptorSetLayout(_vulkanDevice->getLogicalDevice(), &descriptorLayoutCI, nullptr, &_basicDescriptorSetLayout));
	}

	void VulkanRenderer::createUniformBuffers()
	{
		// Prepare and initialize the per-frame uniform buffer blocks containing shader uniforms
		// Single uniforms like in OpenGL are no longer present in Vulkan. All Shader uniforms are passed via uniform buffer blocks
		VkMemoryRequirements memReqs;

		// Vertex shader uniform buffer block
		VkBufferCreateInfo bufferInfo{};
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.allocationSize = 0;
		allocInfo.memoryTypeIndex = 0;

		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = sizeof(ShaderData);
		// This buffer will be used as a uniform buffer
		bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

		// Create the buffers
		for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES; i++) {
			VK_CHECK_RESULT(vkCreateBuffer(_vulkanDevice->getLogicalDevice(), &bufferInfo, nullptr, &_uniformBuffers[i].buffer));
			// Get memory requirements including size, alignment and memory type
			vkGetBufferMemoryRequirements(_vulkanDevice->getLogicalDevice(), _uniformBuffers[i].buffer, &memReqs);
			allocInfo.allocationSize = memReqs.size;
			// Get the memory type index that supports host visible memory access
			// Most implementations offer multiple memory types and selecting the correct one to allocate memory from is crucial
			// We also want the buffer to be host coherent so we don't have to flush (or sync after every update.
			// Note: This may affect performance so you might not want to do this in a real world application that updates buffers on a regular base
			allocInfo.memoryTypeIndex = _vulkanDevice->get_gpu().getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			// Allocate memory for the uniform buffer
			VK_CHECK_RESULT(vkAllocateMemory(_vulkanDevice->getLogicalDevice(), &allocInfo, nullptr, &(_uniformBuffers[i].memory)));
			// Bind memory to buffer
			VK_CHECK_RESULT(vkBindBufferMemory(_vulkanDevice->getLogicalDevice(), _uniformBuffers[i].buffer, _uniformBuffers[i].memory, 0));
			// We map the buffer once, so we can update it without having to map it again
			VK_CHECK_RESULT(vkMapMemory(_vulkanDevice->getLogicalDevice(), _uniformBuffers[i].memory, 0, sizeof(ShaderData), 0, (void**)&_uniformBuffers[i].mapped));
		}
	}

	void VulkanRenderer::updateUniformBuffer()
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		ShaderData ubo{};
		//ubo.modelMatrix = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		
		//ubo.modelMatrix = mymath::rotate(mymath::Mat4(1.0f), time * glm::radians(90.0f), mymath::Vec3(0.0f, 0.0f, 1.0f));
		
		ubo.modelMatrix = mymath::rotate(mymath::Mat4(1.0f), angles[0], mymath::Vec3(1.0f, 0.0f, 0.0f));
		ubo.modelMatrix = mymath::rotate(ubo.modelMatrix, angles[1], mymath::Vec3(0.0f, 1.0f, 0.0f));
		ubo.modelMatrix = mymath::rotate(ubo.modelMatrix, angles[2], mymath::Vec3(0.0f, 0.0f, 1.0f));

		//ubo.modelMatrix = glm::rotate(glm::mat4(1.0f), angles[0], glm::vec3(1.0f, 0.0f, 0.0f));
		//ubo.modelMatrix = glm::rotate(ubo.modelMatrix, angles[1], glm::vec3(0.0f, 1.0f, 0.0f));
		//ubo.modelMatrix = glm::rotate(ubo.modelMatrix, angles[2], glm::vec3(0.0f, 0.0f, 1.0f));

		//ubo.modelMatrix = glm::mat4(1.0f);



		//ubo.viewMatrix = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//ubo.viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		ubo.viewMatrix = mymath::lookAt(mymath::Vec3(2.0f, 2.0f, 2.0f), mymath::Vec3(0.0f, 0.0f, 0.0f), mymath::Vec3(0.0f, 0.0f, 1.0f));
		
		
		//ubo.viewMatrix = mymath::lookAtGLM(mymath::Vec3(2.0f, 2.0f, 2.0f), mymath::Vec3(0.0f, 0.0f, 0.0f), mymath::Vec3(0.0f, -1.0f, 0.0f));
		
		//ubo.viewMatrix = _camera.getView();

		//ubo.viewMatrix = mymath::Mat4(1.0f);
		//ubo.viewMatrix = glm::mat4(1.0f);

		//ubo.projectionMatrix = glm::perspective(glm::radians(45.0f), getAspectRatio(), 0.1f, 100.0f);
		//ubo.projectionMatrix = _camera.getProjection();
		
		ubo.projectionMatrix = mymath::perspective(glm::radians(45.0f), getAspectRatio(), 0.1f, 100.0f);
		//ubo.projectionMatrix = mymath::perspectiveGLM(glm::radians(45.0f), getAspectRatio(), 0.1f, 10.0f);
		
		ubo.projectionMatrix[5] *= -1;
		//ubo.projectionMatrix = glm::mat4(1.0f);


		//ubo.projectionMatrix[1][1] *= -1;
		memcpy(_uniformBuffers[_currentFrame].mapped, &ubo, sizeof(ubo));
	}

	void VulkanRenderer::updateObjectUniformBuffer(VulkanModel* model, mymath::Mat4 worldMat, uint32_t colorMode)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		ShaderData ubo{};
		//ubo.modelMatrix = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		//ubo.modelMatrix = mymath::rotate(mymath::Mat4(1.0f), time * glm::radians(90.0f), mymath::Vec3(0.0f, 0.0f, 1.0f));

		//ubo.modelMatrix = mymath::rotate(mymath::Mat4(1.0f), angles[0], mymath::Vec3(1.0f, 0.0f, 0.0f));
		//ubo.modelMatrix = mymath::rotate(ubo.modelMatrix, angles[1], mymath::Vec3(0.0f, 1.0f, 0.0f));
		//ubo.modelMatrix = mymath::rotate(ubo.modelMatrix, angles[2], mymath::Vec3(0.0f, 0.0f, 1.0f));

		//ubo.modelMatrix = glm::rotate(glm::mat4(1.0f), angles[0], glm::vec3(1.0f, 0.0f, 0.0f));
		//ubo.modelMatrix = glm::rotate(ubo.modelMatrix, angles[1], glm::vec3(0.0f, 1.0f, 0.0f));
		//ubo.modelMatrix = glm::rotate(ubo.modelMatrix, angles[2], glm::vec3(0.0f, 0.0f, 1.0f));

		ubo.modelMatrix = worldMat;

		//ubo.modelMatrix = glm::mat4(1.0f);



		//ubo.viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//ubo.viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		ubo.viewMatrix = mymath::lookAt(mymath::Vec3(0.0f, 0.0f, -8.0f), mymath::Vec3(0.0f, 0.0f, 0.0f), mymath::Vec3(0.0f, 1.0f, 0.0f));


		//ubo.viewMatrix = mymath::lookAtGLM(mymath::Vec3(2.0f, 2.0f, 2.0f), mymath::Vec3(0.0f, 0.0f, 0.0f), mymath::Vec3(0.0f, -1.0f, 0.0f));

		//ubo.viewMatrix = _camera.getView();

		//ubo.viewMatrix = mymath::Mat4(1.0f);
		//ubo.viewMatrix = glm::mat4(1.0f);

		//ubo.projectionMatrix = glm::perspective(glm::radians(45.0f), getAspectRatio(), 0.1f, 100.0f);
		//ubo.projectionMatrix = _camera.getProjection();

		ubo.projectionMatrix = mymath::perspective(glm::radians(45.0f), getAspectRatio(), 0.1f, 100.0f);
		//ubo.projectionMatrix = mymath::perspectiveGLM(glm::radians(45.0f), getAspectRatio(), 0.1f, 10.0f);

		//ubo.projectionMatrix[5] *= -1;
		//ubo.projectionMatrix = glm::mat4(1.0f);

		ubo.colorMode = colorMode;

		//ubo.projectionMatrix[1][1] *= -1;
		memcpy(_uniformBuffers[_currentFrame].mapped, &ubo, sizeof(ubo));
	}

	void VulkanRenderer::update()
	{
		
		if (scop::Scop::_keymovement.moves.spin_x)
		{
			std::cout << true << std::endl;
			angles[0] += 0.01;
		}
		else if (scop::Scop::_keymovement.moves.spin_y)
		{
			angles[1] += 0.01;
		}
		else if (scop::Scop::_keymovement.moves.spin_z)
		{
			angles[2] += 0.01;
		}
	}

	// Descriptors are allocated from a pool, that tells the implementation how many and what types of descriptors we are going to use (at maximum)
	void VulkanRenderer::createDescriptorPool()
	{
		std::array<VkDescriptorPoolSize, 2> descriptorPoolSizes{};

		// We need to tell the API the number of max. requested descriptors per type
		
		// This example only one descriptor type (uniform buffer)
		descriptorPoolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		// We have one buffer (and as such descriptor) per frame
		descriptorPoolSizes[0].descriptorCount = MAX_CONCURRENT_FRAMES;

		// Combined image samples : 1 per object texture
		descriptorPoolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorPoolSizes[1].descriptorCount = MAX_CONCURRENT_FRAMES;


		// For additional types you need to add new entries in the type count list
		// E.g. for two combined image samplers :
		// typeCounts[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		// typeCounts[1].descriptorCount = 2;

		// Create the global descriptor pool
		// All descriptors used in this example are allocated from this pool
		VkDescriptorPoolCreateInfo descriptorPoolCI{};
		descriptorPoolCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCI.pNext = nullptr;
		descriptorPoolCI.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
		descriptorPoolCI.pPoolSizes = descriptorPoolSizes.data();
		// Set the max. number of descriptor sets that can be requested from this pool (requesting beyond this limit will result in an error)
		// Our sample will create one set per uniform buffer per frame
		descriptorPoolCI.maxSets = MAX_CONCURRENT_FRAMES;
		VK_CHECK_RESULT(vkCreateDescriptorPool(_vulkanDevice->getLogicalDevice(), &descriptorPoolCI, nullptr, &_basicDescriptorPool));

	}

	// Shaders access data using descriptor sets that "point" at our uniform buffers
	// The descriptor sets make use of the descriptor set layouts created above 
	void VulkanRenderer::createDescriptorSets()
	{
		// Allocate one descriptor set per frame from the global descriptor pool
		for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES; i++) {
			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = _basicDescriptorPool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &_basicDescriptorSetLayout;
			VK_CHECK_RESULT(vkAllocateDescriptorSets(_vulkanDevice->getLogicalDevice(), &allocInfo, &_uniformBuffers[i].descriptorSet));

			// Update the descriptor set determining the shader binding points
			// For every binding point used in a shader there needs to be one
			// descriptor set matching that binding point
			VkWriteDescriptorSet writeDescriptorSet{};
			std::array<VkWriteDescriptorSet, 2> writeDescriptorSets{};

			// The buffer's information is passed using a descriptor info structure
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = _uniformBuffers[i].buffer;
			bufferInfo.range = sizeof(ShaderData);

			// Setup a descriptor image info for the current texture to be used as a combined image sampler
			VkDescriptorImageInfo textureDescriptor;
			// The image's view (images are never directly accessed by the shader, but rather through views defining subresources)
			textureDescriptor.imageView = _texture->_TextureImageView;
			// The sampler (Telling the pipeline how to sample the texture, including repeat, border, etc.)
			textureDescriptor.sampler = _texture->_TextureSampler;
			// The current layout of the image(Note: Should always fit the actual use, e.g.shader read)
			textureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			// Binding 0 : Uniform buffer
			writeDescriptorSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeDescriptorSets[0].dstSet = _uniformBuffers[i].descriptorSet;
			writeDescriptorSets[0].descriptorCount = 1;
			writeDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writeDescriptorSets[0].pBufferInfo = &bufferInfo;
			writeDescriptorSets[0].dstBinding = 0;

			/*
				Binding 1: Object texture
			*/
			writeDescriptorSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeDescriptorSets[1].dstSet = _uniformBuffers[i].descriptorSet;
			writeDescriptorSets[1].dstBinding = 1;
			writeDescriptorSets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			// Images use a different descriptor structure, so we use pImageInfo instead of pBufferInfo
			writeDescriptorSets[1].pImageInfo = &textureDescriptor;
			writeDescriptorSets[1].descriptorCount = 1;

			vkUpdateDescriptorSets(_vulkanDevice->getLogicalDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
		}
	}

	void VulkanRenderer::loadTexture()
	{
		const VulkanQueue& queue = _vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);


		std::string filename{ "textures/lena.bmp" };
		_texture = new VulkanTexture(filename, queue.get_queue(), _vulkanDevice);

	}

}

