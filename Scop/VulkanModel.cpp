#include "VulkanModel.h"
#include "VulkanTools.h"
#include "VulkanRenderer.h"

namespace vks
{
	VulkanModel::VulkanModel()
	{
	}


	bool VulkanModel::Initialize(VulkanRenderer* renderer)
	{
		_renderer = renderer;

		return false;
	}

	VulkanModel::~VulkanModel()
	{
		_VertexBuffer.destroy();
		_IndexBuffer.destroy();
	}

	void VulkanModel::bind(VkCommandBuffer commandBuffer)
	{
		VkDeviceSize offsets[1]{ 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &_VertexBuffer.buffer, offsets);
		// Bind triangle index buffer

		vkCmdBindIndexBuffer(commandBuffer, _IndexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
	}

	void VulkanModel::draw(VkCommandBuffer commandBuffer)
	{
		// Draw indexed triangle
		vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 1);

		// TODO
		//vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	void VulkanModel::draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout)
	{

	}

	void VulkanModel::createVertexBuffer(std::vector<vks::VulkanModel::Vertex>& vertices)
	{
		// A note on memory management in Vulkan in general:
			//	This is a very complex topic and while it's fine for an example application to small individual memory allocations that is not
			//	what should be done a real-world application, where you should allocate large chunks of memory at once instead.

			// Setup vertices
		/*std::vector<Vertex> vertices{
			{ {  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
			{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
			{ {  0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
		};*/

		/*std::vector<Vertex> vertices{
			{ {  0.0f,  -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
			{ { 0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
			{ {  -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
		};*/
		
		/*
		_Vertices = {
			{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

			{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
			{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
		};

		_Indices = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};
		*/
		
		_Vertices = vertices;

		uint32_t vertexBufferSize = static_cast<uint32_t>(_Vertices.size()) * sizeof(Vertex);
		vertexCount = _Vertices.size();

		// Setup indices
		//std::vector<uint32_t> indices{ 0, 1, 2 };

		
		// Create buffers and upload data to the GPU
		struct StagingBuffers {
			vks::Buffer vertices;
			vks::Buffer indices;
		} stagingBuffers;

		VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();

		// Host visible source buffers (staging)
		VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffers.vertices, vertexBufferSize, _Vertices.data()));
		
		// Device local destination buffers
		VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &_VertexBuffer, _Vertices.size() * sizeof(Vertex)));
		
		const VulkanQueue& queue = vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);

		// Copy from host do device
		vulkanDevice->copyBuffer(&stagingBuffers.vertices, &_VertexBuffer, queue.get_queue());
	
		// Clean up
		stagingBuffers.vertices.destroy();
	}

	void VulkanModel::createIndexBuffer(std::vector<uint32_t>& indices)
	{
		_Indices = indices;

		indexCount = static_cast<uint32_t>(_Indices.size());

		struct StagingBuffers {
			vks::Buffer vertices;
			vks::Buffer indices;
		} stagingBuffers;

		VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();
		// Host visible source buffers (staging)
		VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffers.indices, _Indices.size() * sizeof(uint32_t), _Indices.data()));
		
		// Device local destination buffers
		VK_CHECK_RESULT(vulkanDevice->createBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &_IndexBuffer, _Indices.size() * sizeof(uint32_t)));

		const VulkanQueue& queue = vulkanDevice->get_queue_by_flags(VK_QUEUE_GRAPHICS_BIT, 0);

		// Copy from host do device
		vulkanDevice->copyBuffer(&stagingBuffers.indices, &_IndexBuffer, queue.get_queue());

		// Clean up
		stagingBuffers.indices.destroy();
	}

	void VulkanModel::EndCreateMesh()
	{
	}

	std::vector<VkVertexInputBindingDescription> VulkanModel::Vertex::getBindingDescription()
	{
		// Vertex input binding
			// This example uses a single vertex input binding at binding point 0 (see vkCmdBindVertexBuffers)
		std::vector<VkVertexInputBindingDescription> vertexInputBinding(1);

		//VkVertexInputBindingDescription vertexInputBinding{};
		vertexInputBinding[0].binding = 0;
		vertexInputBinding[0].stride = sizeof(Vertex);
		vertexInputBinding[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return vertexInputBinding;
	}

	std::vector<VkVertexInputAttributeDescription> VulkanModel::Vertex::getAttributeDescription()
	{
		std::vector<VkVertexInputAttributeDescription> vertexInputAttributs{};

		vertexInputAttributs.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT , offsetof(Vertex, position) });
		vertexInputAttributs.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT , offsetof(Vertex, color) });
		vertexInputAttributs.push_back({ 2, 0, VK_FORMAT_R32G32_SFLOAT , offsetof(Vertex, uv) });
		vertexInputAttributs.push_back({ 3, 0, VK_FORMAT_R32G32B32_SFLOAT , offsetof(Vertex, normal) });
		
		return vertexInputAttributs;
	}

}
