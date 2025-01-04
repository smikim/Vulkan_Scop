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
#include "BmpLoader.h"

#include "Camera.h"
#include "Matrix.h"
#include "Types.h"

#include <chrono>

#include <vector>

#define MAX_CONCURRENT_FRAMES 2

class ScopObject;

namespace vks
{
	class VulkanDevice;

	class VulkanRenderer
	{
	public:
		VulkanRenderer(GlfwWindow& window);
		
		~VulkanRenderer();

		bool initVulkan();
		
		IVulkanModel* CreateBasicMeshObject();
		void BeginCreateMesh(IVulkanModel* model, std::vector<vks::VulkanModel::Vertex>& vertices);
		void InsertIndexBuffer(IVulkanModel* model, std::vector<uint32_t>& indices);	
		void EndCreateMesh(IVulkanModel* model, std::string& BmpFilename);
		void DeleteMeshObject(IVulkanModel* model);


		VulkanTexture* CreateTexture(std::string& filename);
		void init_basicPipeline(VkPipelineLayout pipelineLayout);
		
		VkResult beginRender();
		void beginRenderPass();
		void endRenderPass();
		VkResult endRender();
		void renderMeshObject(IVulkanModel* object, mymath::Mat4 worldMat, uint32_t colorMode);

		VkResult prepareFrame();
		VkResult submitFrame();
		

		uint32_t getWidth() { return _width; }
		uint32_t getHeight() { return _height; }

		VkFramebuffer get_frameBuffer_by_index(uint32_t index) const {
			return _FrameBuffers[index];
		}

		VkCommandBuffer getCurrentCommandBuffer() const {		
			return _drawCommandBuffer->get_commandBuffer_by_index(_currentImageIndex);
		}

		vks::VulkanDevice* getVulkanDevice() const {
			return _vulkanDevice;
		}

		float getAspectRatio() { 
			return static_cast<float>(_width) / static_cast<float>(_height); 
		};

		std::array<UniformBuffer, MAX_CONCURRENT_FRAMES> createUniformBuffers();
		
		Graphics::BasicPSO* _basicPSO;
		VulkanPipeline* _basicPipeline;

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

		
		void setupRenderPass();
		void setupDepthStencil(uint32_t& width, uint32_t& height);
		void setupFrameBuffer(uint32_t& width, uint32_t& height);
		void createCommandPool(); 
		
		void createSynchronizationPrimitives();

		VkResult submitCommandBuffer();
		void windowResize();
		
		void updateObjectUniformBuffer(IVulkanModel* model, mymath::Mat4 worldMat, uint32_t colorMode);

		uint32_t _width;
		uint32_t _height;
		
		bool _prepared = false;
		bool _resized = false;

		VkClearColorValue _defaultClearColor = { { 0.0f, 0.0f, 0.2f, 1.0f } };

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

		// Semaphores are used to coordinate operations within the graphics queue and ensure correct command ordering
		std::array<VkSemaphore, MAX_CONCURRENT_FRAMES> _PresentCompleteSemaphores{};
		std::array<VkSemaphore, MAX_CONCURRENT_FRAMES> _RenderCompleteSemaphores{};

		std::array<VkFence, MAX_CONCURRENT_FRAMES> _WaitFences{};

		// Command buffers used for rendering
		VulkanCommandBuffer* _drawCommandBuffer;
	};
}