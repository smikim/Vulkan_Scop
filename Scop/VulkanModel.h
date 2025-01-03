#pragma once
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "Vector.h"
#include "Matrix.h"

namespace vks
{
	class VulkanRenderer;
	class VulkanTexture;

	// TODO 
	// class IModel 인터페이스 생성

	class IVulkanModel
	{
	public:
		struct Vertex {
			float position[3];
			float color[3];
			float uv[2];
			float normal[3];
			uint32_t triangleID; // 삼각형 ID

			static std::vector<VkVertexInputBindingDescription> getBindingDescription();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
		};

		virtual ~IVulkanModel() = default;

		virtual bool Initialize(VulkanRenderer* renderer) = 0;
		virtual void bind(VkCommandBuffer commandBuffer) = 0;
		virtual void draw(VkCommandBuffer commandBuffer) = 0;
		virtual void createVertexBuffer(std::vector<Vertex>& vertices) = 0;
		virtual void createIndexBuffer(std::vector<uint32_t>& indices) = 0;
		virtual void EndCreateMesh() = 0;
	};

	class VulkanModel : public IVulkanModel
	{
	public:
		//struct Vertex {
		//	float position[3];
		//	float color[3];
		//	float uv[2];
		//	float normal[3];
		//	uint32_t	 triangleID; // 삼각형 ID

		//	static std::vector<VkVertexInputBindingDescription> getBindingDescription();
		//	static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
		//};
	public:
		VulkanModel();
		virtual ~VulkanModel();

		VulkanModel(const VulkanModel&) = delete;
		VulkanModel& operator=(const VulkanModel&) = delete;


		bool	Initialize(VulkanRenderer* renderer) override;

		void bind(VkCommandBuffer commandBuffer);
		virtual void draw(VkCommandBuffer commandBuffer) override;
		void createVertexBuffer(std::vector<vks::VulkanModel::Vertex>& vertices) override;
		void createIndexBuffer(std::vector<uint32_t>& indices) override;
		void EndCreateMesh() override;

	protected:
		VulkanRenderer* _renderer;

	private:


		Buffer _VertexBuffer;
		uint32_t vertexCount;
		Buffer _IndexBuffer;

		uint32_t indexCount;

		std::vector<Vertex> _Vertices;
		std::vector<uint32_t> _Indices;
	};

}

