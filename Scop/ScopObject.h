#pragma once

#include "VulkanModel.h"
#include "Vector.h"
#include "Matrix.h"

namespace scop
{
	class Scop;
	class VulkanRenderer;

	struct TransformComponent {
		mymath::Vec3 translation{};
		mymath::Vec3 scale{ 1.f, 1.f, 1.f };
		mymath::Vec3 rotation{};

		bool spinAtX = false;
		bool spinAtY = false;
		bool spinAtZ = false;

		bool spinOnX = false;
		bool spinOnY = false;
		bool spinOnZ = false;

		mymath::Mat4 matTrans{ 1.0f };
		mymath::Mat4 matRot{ 1.0f };
		mymath::Mat4 matScale{ 1.0f };

		mymath::Mat4 worldMatrix{ 1.0f };


		// Matrix correspons to translate * Ry * Rx * Rz * scale transformation
		// Rotation convention uses tait-bryan angles with axis order Y(1), X(2), Z(3)
		 // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
		// Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
		// https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
		
		mymath::Mat4 mat4();
		mymath::Mat4 getWorldMatrix();
		mymath::Mat4 normalMatrix();

	};

	class ScopObject
	{
		Scop* _scop;
		vks::VulkanRenderer* _renderer;
		// TODO 
		// class IModel 로 추상화 하여, VulkanRenderer로만 접근 가능하게 변경
		vks::IVulkanModel* _vulkanModel;

		

		// TODO
		// obj file에서 데이터 읽어 오는 거로 확장
		vks::IVulkanModel* CreateBoxMeshObject();
		vks::IVulkanModel* CreateObjMeshObject(std::string& filename);

		void	UpdateTransform();
		void	Cleanup();
	public:
		TransformComponent _transform{ };
		uint32_t			_colorMode{3};

		ScopObject();
		~ScopObject();
		bool	Initialize(Scop* scop, std::string& filename);
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


