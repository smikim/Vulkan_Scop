#pragma once

#include "VulkanModel.h"

namespace scop
{
	class Scop;
	class VulkanRenderer;

	struct TransformComponent {
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f, 1.f, 1.f };
		glm::vec3 rotation{};

		bool spinAtX = false;
		bool spinAtY = false;
		bool spinAtZ = false;

		bool spinOnX = false;
		bool spinOnY = false;
		bool spinOnZ = false;

		glm::mat4 matTrans{1.0f};
		glm::mat4 matRot{1.0f};
		glm::mat4 matScale{ 1.0f };

		glm::mat4 worldMatrix{ 1.0f };
		// Matrix correspons to translate * Ry * Rx * Rz * scale transformation
		// Rotation convention uses tait-bryan angles with axis order Y(1), X(2), Z(3)
		 // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
		// Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
		// https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
		
		glm::mat4 mat4();
		glm::mat4 getWorldMatrix();
		glm::mat3 normalMatrix();

	};

	class ScopObject
	{
		Scop* _scop;
		vks::VulkanRenderer* _renderer;
		// TODO 
		// class IModel 로 추상화 하여, VulkanRenderer로만 접근 가능하게 변경
		vks::VulkanModel* _vulkanModel;

		

		// TODO
		// obj file에서 데이터 읽어 오는 거로 확장
		vks::VulkanModel* CreateBoxMeshObject();


		void	UpdateTransform();
		void	Cleanup();
	public:
		TransformComponent _transform{ };

		ScopObject();
		~ScopObject();
		bool	Initialize(Scop* scop);
		void	setTranslation(float x, float y, float z);
		void	setScale(float x, float y, float z);
		void	setRotation(float x, float y, float z);
		
		void	moveRotation(float x, float y, float z);
		void	moveTranslation(float x, float y, float z);
		void	Run();
		void	Render();
	};

	// temporary helper function, creates a 1x1x1 cube centered at offset
	void createCubeData(std::vector<vks::VulkanModel::Vertex>& vertices,
		std::vector<uint32_t>& indices);
}


