#include "VulkanModel.h"
#include "VulkanTools.h"

namespace vks
{
	VulkanModel::VulkanModel(VulkanDevice& device)
		: _Device(device)
	{
	}

	VulkanModel::VulkanModel(VulkanDevice& device, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
		: _Device(device), _Vertices(std::move(vertices)), _Indices(std::move(indices))
	{
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

		//vkCmdBindIndexBuffer(commandBuffer, _IndexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
	}

	void VulkanModel::draw(VkCommandBuffer commandBuffer)
	{
		// Draw indexed triangle
		//vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 1);

		// TODO
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	void VulkanModel::draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout)
	{

	}

	void VulkanModel::createVertexBuffer(VkQueue queue)
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

		_Vertices = {
			{{0.0f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
		};
		
		uint32_t vertexBufferSize = static_cast<uint32_t>(_Vertices.size()) * sizeof(Vertex);
		vertexCount = _Vertices.size();

		// Setup indices
		//std::vector<uint32_t> indices{ 0, 1, 2 };

		indexCount = static_cast<uint32_t>(_Indices.size());


		// Create buffers and upload data to the GPU
		struct StagingBuffers {
			vks::Buffer vertices;
			vks::Buffer indices;
		} stagingBuffers;

		// Host visible source buffers (staging)
		VK_CHECK_RESULT(_Device.createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffers.vertices, vertexBufferSize, _Vertices.data()));
		//VK_CHECK_RESULT(_Device.createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffers.indices, _Indices.size() * sizeof(uint32_t), _Indices.data()));

		// Device local destination buffers
		VK_CHECK_RESULT(_Device.createBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &_VertexBuffer, _Vertices.size() * sizeof(Vertex)));
		//VK_CHECK_RESULT(_Device.createBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &_IndexBuffer, _Indices.size() * sizeof(uint32_t)));

		// Copy from host do device
		_Device.copyBuffer(&stagingBuffers.vertices, &_VertexBuffer, queue);
		//_Device.copyBuffer(&stagingBuffers.indices, &_IndexBuffer, queue);

		// Clean up
		stagingBuffers.vertices.destroy();
		//stagingBuffers.indices.destroy();

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
		
		return vertexInputAttributs;
	}

}
