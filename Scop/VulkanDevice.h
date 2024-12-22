#pragma once

#include "VulkanPhysicalDevice.h"
#include "VulkanQueue.h"
#include "VulkanBuffer.h"
#include <unordered_map>

namespace vks
{
	class VulkanDevice
	{
	public:
		VulkanDevice(VulkanPhysicalDevice& gpu, VkSurfaceKHR surface, std::unordered_map<const char*, bool> requested_extensions);
		~VulkanDevice();

		VkResult				createLogicalDevice(VkSurfaceKHR surface, VkPhysicalDeviceFeatures enabledFeatures, std::vector<const char*> enabledExtensions, void* pNextChain, std::unordered_map<const char*, bool> requested_extensions, bool useSwapChain = true, VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
		
		uint32_t				get_queue_family_index(VkQueueFlagBits queue_flag);
		const VulkanQueue&		get_queue_by_flags(VkQueueFlags required_queue_flags, uint32_t queue_index) const;


		bool					is_extension_supported(const std::string& extension) const;
		void					create_graphics_queue(VkSurfaceKHR surface);

		VkDevice				getLogicalDevice() const;
		const VulkanPhysicalDevice& get_gpu() const;

		/**
		 * @brief Finds a suitable graphics queue to submit to
		 * @return The first present supported queue, otherwise just any graphics queue
		 */
		const VulkanQueue&		get_suitable_graphics_queue() const;

		const VulkanQueue&		get_queue_by_present(uint32_t queue_index) const;
		
		VkCommandPool			get_command_pool() const;

		VkResult				createBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, vks::Buffer* buffer, VkDeviceSize size, void* data = nullptr);
		void					copyBuffer(vks::Buffer* src, vks::Buffer* dst, VkQueue queue, VkBufferCopy* copyRegion = nullptr);

		VkCommandBuffer			createCommandBuffer(VkCommandBufferLevel level, VkCommandPool pool, bool begin = false);

		VkCommandBuffer			createCommandBuffer(VkCommandBufferLevel level, bool begin = false);
		void					flushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, VkCommandPool pool, bool free = true);
		void					flushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, bool free = true);

	private:
		VkCommandPool			createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);


		const VulkanPhysicalDevice& _gpu;
		/** @brief Logical device representation (application's view of the device) */

		VkDevice _LogicalDevice;

		/** @brief Set of physical device features to be enabled for this example (must be set in the derived constructor) */
		VkPhysicalDeviceFeatures enabledFeatures{};

		/** @brief Set of device extensions to be enabled for this example (must be set in the derived constructor) */
		std::vector<const char*> enabledDeviceExtensions;

		/** @brief Optional pNext structure for passing extension structures to device creation */
		void* deviceCreatepNextChain = nullptr;
		
		std::vector<std::vector<VulkanQueue>> queues;

		/** @brief Default command pool for the graphics queue family index */
		VkCommandPool _CommandPool = VK_NULL_HANDLE;
	};

}

