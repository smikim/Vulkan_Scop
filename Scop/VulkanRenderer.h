#pragma once
#include "VulkanInstance.h"
#include "GlfwWindow.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "BasicPSO.h"
#include "VulkanPipelineState.h"
#include "VulkanCommandBuffer.h"
#include "VulkanModel.h"
#include "VulkanTexture.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

#include <vector>

#define MAX_CONCURRENT_FRAMES 2

namespace vks
{
	class VulkanDevice;

	class VulkanRenderer
	{
	public:

		// Uniform buffer block object
		struct UniformBuffer {
			VkDeviceMemory memory;
			VkBuffer buffer;
			// The descriptor set stores the resources bound to the binding points in a shader
			// It connects the binding points of the different shaders with the buffers and images used for those bindings
			VkDescriptorSet descriptorSet;
			// We keep a pointer to the mapped buffer, so we can easily update it's contents via a memcpy
			uint8_t* mapped{ nullptr };
		};
		// We use one UBO per frame, so we can have a frame overlap and make sure that uniforms aren't updated while still in use
		std::array<UniformBuffer, MAX_CONCURRENT_FRAMES> _uniformBuffers;

		// For simplicity we use the same uniform block layout as in the shader:
		//
		//	layout(set = 0, binding = 0) uniform UBO
		//	{
		//		mat4 projectionMatrix;
		//		mat4 modelMatrix;
		//		mat4 viewMatrix;
		//	} ubo;
		//
		// This way we can just memcopy the ubo data to the ubo
		// Note: You should use data types that align with the GPU in order to avoid manual padding (vec4, mat4)
		struct ShaderData {
			glm::mat4 modelMatrix;
			glm::mat4 viewMatrix;
			glm::mat4 projectionMatrix;
		}; 
		
		VulkanRenderer(GlfwWindow& window);
		~VulkanRenderer();

		bool initVulkan();
		void init_basicPipeline(Graphics::BasicPSO* basicPSO, VkPipelineLayout pipelineLayout);
		
		void buildBasicCommandBuffers();
		VkResult prepareFrame();
		VkResult submitFrame();
		

		uint32_t getWidth() { return _width; }
		uint32_t getHeight() { return _height; }

		VkFramebuffer get_frameBuffer_by_index(uint32_t index) const {
			return _FrameBuffers[index];
		}

		VkCommandBuffer getCurrentCommandBuffer() const {
			//assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return _drawCommandBuffer->get_commandBuffer_by_index(_currentImageIndex);
			//return _drawCommandBuffer->get_commandBuffer_by_index(_currentFrame);
			//return drawCmdBuffers[currentFrame];
		}

		vks::VulkanDevice* getVulkanDevice() const {
			return _vulkanDevice;
		}

		float getAspectRatio() { return static_cast<float>(_width) / static_cast<float>(_height); };

		// TODO
		void updateUniformBuffer();

		Graphics::BasicPSO* _basicPSO;
		VulkanPipeline* _basicPipeline;
		VkPipelineLayout _basicPipelineLayout{ VK_NULL_HANDLE };

		

	private:
		// TODO
		void getEnabledFeatures();
		void create_surface();

		void add_device_extension(const char* extension, bool optional = false)
		{
			device_extensions[extension] = optional;
		}


		std::unordered_map<const char*, bool> const& get_device_extensions() const
		{
			return device_extensions;
		}

		void createPipelineLayout();
		void setupRenderPass();
		void setupDepthStencil(uint32_t& width, uint32_t& height);
		void setupFrameBuffer(uint32_t& width, uint32_t& height);
		void createCommandPool(); 
		
		void createSynchronizationPrimitives();

		VkResult submitCommandBuffer();
		void windowResize();

		void createDescriptorSetLayout();
		void createUniformBuffers();
		
		void createDescriptorPool();
		void createDescriptorSets();

		uint32_t _width;
		uint32_t _height;
		
		bool _prepared = false;
		bool _resized = false;

		VkClearColorValue _defaultClearColor = { { 0.025f, 0.025f, 0.025f, 1.0f } };

		// To select the correct sync objects, we need to keep track of the current frame
		uint32_t _currentFrame{ 0 };
		uint32_t _currentImageIndex{ 0 };

		vks::VulkanInstance _instance;
		GlfwWindow& _window;
		VkSurfaceKHR _Surface{ VK_NULL_HANDLE };
		vks::VulkanDevice* _vulkanDevice;

		/** @brief Set of device extensions to be enabled for this example and whether they are optional (must be set in the derived constructor) */
		std::unordered_map<const char*, bool> device_extensions;

		// Wraps the swap chain to present images (framebuffers) to the windowing system
		std::unique_ptr<VulkanSwapChain> _swapChain;
		VkRenderPass _RenderPass{ VK_NULL_HANDLE };
		
		// Depth buffer format (selected during Vulkan initialization)
		VkFormat _DepthFormat;
		bool _requiresStencil{ false };
		/** @brief Default depth stencil attachment used by the default render pass */
		struct {
			VkImage image;
			VkDeviceMemory memory;
			VkImageView view;
		} _depthStencil{};

		// List of available frame buffers (same as number of swap chain images)
		std::vector<VkFramebuffer>_FrameBuffers;

		// Command buffer pool
		VkCommandPool _CmdPool{ VK_NULL_HANDLE };

		// Command buffers used for rendering
		//std::vector<VkCommandBuffer> _drawCmdBuffers;

		// Semaphores are used to coordinate operations within the graphics queue and ensure correct command ordering
		std::array<VkSemaphore, MAX_CONCURRENT_FRAMES> _PresentCompleteSemaphores{};
		std::array<VkSemaphore, MAX_CONCURRENT_FRAMES> _RenderCompleteSemaphores{};

		std::array<VkFence, MAX_CONCURRENT_FRAMES> _WaitFences{};

		// Command buffers used for rendering
		VulkanCommandBuffer* _drawCommandBuffer;

		VkDescriptorSetLayout _basicDescriptorSetLayout{ VK_NULL_HANDLE };
		VkDescriptorPool _basicDescriptorPool{ VK_NULL_HANDLE };
		// TODO 
		VulkanModel *_model;
		VulkanTexture* _texture;

	};
}