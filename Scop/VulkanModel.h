#pragma once
#include "VulkanDevice.h"
#include "VulkanBuffer.h"

namespace vks
{
	class VulkanModel
	{
	public:
		struct Vertex {
			float position[3];
			float color[3];
			float uv[2];

			static std::vector<VkVertexInputBindingDescription> getBindingDescription();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
		};
	public:
		VulkanModel(VulkanDevice& device);
		VulkanModel(VulkanDevice& device, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
		virtual ~VulkanModel();

		VulkanModel(const VulkanModel&) = delete;
		VulkanModel& operator=(const VulkanModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		virtual void draw(VkCommandBuffer commandBuffer);

		virtual void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout);

		void createVertexBuffer(VkQueue queue);

	protected:
		VulkanDevice& _Device;

	private:


		Buffer _VertexBuffer;
		uint32_t vertexCount;
		Buffer _IndexBuffer;

		uint32_t indexCount;

		std::vector<Vertex> _Vertices;
		std::vector<uint32_t> _Indices;
	};

}

