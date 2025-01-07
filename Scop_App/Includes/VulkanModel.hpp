#pragma once
#include "../Includes/VulkanDevice.hpp"
#include "../Includes/VulkanBuffer.hpp"
#include "../Library/Math/Vector.hpp"
#include "../Library/Math/Matrix.hpp"
#include "../Includes/Types.hpp"


#define MAX_CONCURRENT_FRAMES 2

namespace vks
{
	class VulkanRenderer;
	class VulkanTexture;

	// TODO 
	// class IModel �������̽� ����

	class IVulkanModel
	{
	public:
		struct Vertex {
			float position[3];
			float color[3];
			float uv[2];
			float normal[3];
			uint32_t triangleID; // �ﰢ�� ID

			static std::vector<VkVertexInputBindingDescription> getBindingDescription();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
		};

		virtual ~IVulkanModel() = default;

		virtual bool Initialize(VulkanRenderer* renderer) = 0;
		virtual bool Initialize(VulkanRenderer* renderer, std::string& ObjFilename) = 0;
		virtual void bind(VkCommandBuffer commandBuffer, uint32_t currentFrame) = 0;
		virtual void draw(VkCommandBuffer commandBuffer) = 0;
		virtual void createVertexBuffer() = 0;
		virtual void createIndexBuffer() = 0;
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
		bool	Initialize(VulkanRenderer* renderer, std::string& ObjFilename) override;

		void bind(VkCommandBuffer commandBuffer, uint32_t currentFrame) override;
		virtual void draw(VkCommandBuffer commandBuffer) override;
		void createVertexBuffer() override;
		void createIndexBuffer() override;
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

		VulkanTexture* _texture = nullptr;

		VkDescriptorSetLayout _basicDescriptorSetLayout{ VK_NULL_HANDLE };
		VkDescriptorPool _basicDescriptorPool{ VK_NULL_HANDLE };
		VkPipelineLayout _basicPipelineLayout{ VK_NULL_HANDLE };

		
		// We use one UBO per frame, so we can have a frame overlap and make sure that uniforms aren't updated while still in use
		std::array<UniformBuffer, MAX_CONCURRENT_FRAMES> _uniformBuffers;


	};

}

