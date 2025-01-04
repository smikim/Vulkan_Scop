#pragma once
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "Vector.h"
#include "Matrix.h"
#include "Types.h"

#define MAX_CONCURRENT_FRAMES 2

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
		virtual void bind(VkCommandBuffer commandBuffer, uint32_t currentFrame) = 0;
		virtual void bind(VkCommandBuffer commandBuffer) {}
		virtual void draw(VkCommandBuffer commandBuffer) = 0;
		virtual void createVertexBuffer(std::vector<Vertex>& vertices) = 0;
		virtual void createIndexBuffer(std::vector<uint32_t>& indices) = 0;
		virtual void EndCreateMesh(std::string& BmpFilename) = 0;

	};

	class VulkanModel : public IVulkanModel
	{
	public:
		VulkanModel();
		virtual ~VulkanModel();

		VulkanModel(const VulkanModel&) = delete;
		VulkanModel& operator=(const VulkanModel&) = delete;


		bool	Initialize(VulkanRenderer* renderer) override;

		void bind(VkCommandBuffer commandBuffer, uint32_t currentFrame) override;
		virtual void draw(VkCommandBuffer commandBuffer) override;
		void createVertexBuffer(std::vector<vks::VulkanModel::Vertex>& vertices) override;
		void createIndexBuffer(std::vector<uint32_t>& indices) override;
		void EndCreateMesh(std::string& BmpFilename) override;

		void createDescriptorSetLayout();
		void createDescriptorPool();
		void createDescriptorSets();
		void createPipelineLayout();
		
		void updateUniformBuffer(uint32_t currentFrame, ShaderData* src);
		
	
	protected:
		VulkanRenderer* _renderer;

	private:
		Buffer _VertexBuffer;
		uint32_t vertexCount;
		Buffer _IndexBuffer;

		uint32_t indexCount;
		std::vector<Vertex> _Vertices;
		std::vector<uint32_t> _Indices;

		VulkanTexture* _texture;

		VkDescriptorSetLayout _basicDescriptorSetLayout{ VK_NULL_HANDLE };
		VkDescriptorPool _basicDescriptorPool{ VK_NULL_HANDLE };
		VkPipelineLayout _basicPipelineLayout{ VK_NULL_HANDLE };

		
		// We use one UBO per frame, so we can have a frame overlap and make sure that uniforms aren't updated while still in use
		std::array<UniformBuffer, MAX_CONCURRENT_FRAMES> _uniformBuffers;


	};

}

