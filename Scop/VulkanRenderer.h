#pragma once
#include "VulkanInstance.h"
#include "GlfwWindow.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "BasicPSO.h"
#include "VulkanPipelineState.h"
#include <vector>

namespace vks
{

	class VulkanRenderer
	{
	public:
		VulkanRenderer(GlfwWindow& window);
		~VulkanRenderer();

		bool initVulkan();
		void init_basicPipeline(Graphics::BasicPSO* basicPSO, VkPipelineLayout pipelineLayout);

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

		uint32_t _width;
		uint32_t _height;

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
	};
}