#pragma once
#include "VulkanDevice.h"
#include "VulkanBuffer.h"

namespace vks
{
	class VulkanRenderer;
	class VulkanTexture;

	// TODO 
	// class IModel �������̽� ����
	struct Cube {
		struct Matrices {
			// renderer
			glm::mat4 projection;
			glm::mat4 view;
			// contents
			glm::mat4 model;
		} matrices;
		VkDescriptorSet descriptorSet;
		//vks::VulkanTexture texture;
		// �ϴ� �� ���� �ؽ��Ŀ� ������ ���۸� ������ ����
		// 1. descriptorSet : uniformBuffer + texture
		// 
		vks::Buffer uniformBuffer;
		glm::vec3 rotation;
	};

	class VulkanModel
	{
	public:
		struct Vertex {
			float position[3];
			float color[3];
			float uv[2];
			float normal[3];
			uint32_t	 triangleID; // �ﰢ�� ID

			static std::vector<VkVertexInputBindingDescription> getBindingDescription();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
		};
	public:
		VulkanModel();
		bool	Initialize(VulkanRenderer* renderer);

		virtual ~VulkanModel();

		VulkanModel(const VulkanModel&) = delete;
		VulkanModel& operator=(const VulkanModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		virtual void draw(VkCommandBuffer commandBuffer);

		virtual void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout);

		void createVertexBuffer(std::vector<vks::VulkanModel::Vertex>& vertices);
		void createIndexBuffer(std::vector<uint32_t>& indices);
		void EndCreateMesh();

	protected:
		VulkanRenderer* _renderer;
		//VulkanDevice& _Device;

	private:


		Buffer _VertexBuffer;
		uint32_t vertexCount;
		Buffer _IndexBuffer;

		uint32_t indexCount;

		std::vector<Vertex> _Vertices;
		std::vector<uint32_t> _Indices;
	};

}

