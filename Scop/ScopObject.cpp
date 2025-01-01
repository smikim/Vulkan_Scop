#include "ScopObject.h"
#include "Scop.h"

namespace scop
{
	glm::mat4 TransformComponent::mat4()
	{
		const float c3 = glm::cos(rotation.z);
		const float s3 = glm::sin(rotation.z);
		const float c2 = glm::cos(rotation.x);
		const float s2 = glm::sin(rotation.x);
		const float c1 = glm::cos(rotation.y);
		const float s1 = glm::sin(rotation.y);

		return glm::mat4{
			{
				scale.x * (c1 * c3 + s1 * s2 * s3),
				scale.x * (c2 * s3),
				scale.x * (c1 * s2 * s3 - c3 * s1),
				0.0f,
			},
			{
				scale.y * (c3 * s1 * s2 - c1 * s3),
				scale.y * (c2 * c3),
				scale.y * (c1 * c3 * s2 + s1 * s3),
				0.0f,
			},
			{
				scale.z * (c2 * s1),
				scale.z * (-s2),
				scale.z * (c1 * c2),
				0.0f,
			},
			{translation.x, translation.y, translation.z, 1.0f} };

	}
	
	glm::mat4 TransformComponent::getWorldMatrix()
	{
		return worldMatrix;

		glm::mat4 mat{ 1.0 };
		mat = glm::translate(mat, translation);
		mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		mat = glm::scale(mat, scale);

		return mat;
	}

	glm::mat3 TransformComponent::normalMatrix()
	{
		const float c3 = glm::cos(rotation.z);
		const float s3 = glm::sin(rotation.z);
		const float c2 = glm::cos(rotation.x);
		const float s2 = glm::sin(rotation.x);
		const float c1 = glm::cos(rotation.y);
		const float s1 = glm::sin(rotation.y);
		const glm::vec3 invScale = 1.0f / scale;

		return glm::mat3{
			{
				invScale.x * (c1 * c3 + s1 * s2 * s3),
				invScale.x * (c2 * s3),
				invScale.x * (c1 * s2 * s3 - c3 * s1),
			},
			{
				invScale.y * (c3 * s1 * s2 - c1 * s3),
				invScale.y * (c2 * c3),
				invScale.y * (c1 * c3 * s2 + s1 * s3),
			},
			{
				invScale.z * (c2 * s1),
				invScale.z * (-s2),
				invScale.z * (c1 * c2),
			},
		};
	}

	vks::VulkanModel* ScopObject::CreateBoxMeshObject()
	{
		vks::VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();

		std::vector<vks::VulkanModel::Vertex> vertices;
		std::vector<uint32_t> indices;
		createCubeData(vertices, indices);
		
		_vulkanModel = _renderer->CreateBasicMeshObject();

		// TODO
		// Texture 추가 하는 코드
		_renderer->BeginCreateMesh(_vulkanModel, vertices);
		_renderer->InsertIndexBuffer(_vulkanModel, indices);
		_renderer->EndCreateMesh(_vulkanModel);

		return _vulkanModel;
	}

	void ScopObject::UpdateTransform()
	{
		_transform.worldMatrix = _transform.matTrans * _transform.matRot;
		_transform.worldMatrix = _transform.worldMatrix * _transform.matScale;
	}

	void ScopObject::Cleanup()
	{
		if (_vulkanModel)
		{
			_renderer->DeleteMeshObject(_vulkanModel);
		}
	}

	ScopObject::ScopObject()
	{
	}

	ScopObject::~ScopObject()
	{
		Cleanup();
	}
	
	bool ScopObject::Initialize(Scop* scop)
	{
		_scop = scop;
		_renderer = scop->getVulkanRenderer();

		_vulkanModel = CreateBoxMeshObject();
		return false;
	}

	void ScopObject::setTranslation(float x, float y, float z)
	{
		_transform.translation.x = x;
		_transform.translation.y = y;
		_transform.translation.z = z;

		_transform.matTrans = glm::translate(_transform.matTrans, _transform.translation);
	}

	void ScopObject::setScale(float x, float y, float z)
	{
		_transform.scale.x = x;
		_transform.scale.y = y;
		_transform.scale.z = z;
		
		_transform.matScale = glm::scale(_transform.matScale, _transform.scale);
	}

	void ScopObject::setRotation(float x, float y, float z)
	{
		_transform.rotation.x = x;
		_transform.rotation.y = y;
		_transform.rotation.z = z;

		_transform.matRot = glm::rotate(_transform.matRot, glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));
		_transform.matRot = glm::rotate(_transform.matRot, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
		_transform.matRot = glm::rotate(_transform.matRot, glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f));

	}

	void ScopObject::moveRotation(float x, float y, float z)
	{
		_transform.rotation.x += x;
		_transform.rotation.y += y;
		_transform.rotation.z += z;

		_transform.matRot = glm::rotate(_transform.matRot, glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));
		_transform.matRot = glm::rotate(_transform.matRot, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
		_transform.matRot = glm::rotate(_transform.matRot, glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f));

	}

	void ScopObject::moveTranslation(float x, float y, float z)
	{
		_transform.translation.x += x;
		_transform.translation.y += y;
		_transform.translation.z += z;

		_transform.matTrans = glm::translate(_transform.matTrans, _transform.translation);


	}

	void ScopObject::Run()
	{
		//UpdateTransform();
		_renderer->updateObjectUniformBuffer(_vulkanModel, _transform.mat4());
	}

	void ScopObject::Render()
	{
		if (_vulkanModel)
		{
			_renderer->renderMeshObject(_vulkanModel);
		}
	}

	void createCubeData(std::vector<vks::VulkanModel::Vertex>& vertices,
		std::vector<uint32_t>& indices) {

		vertices = {
			{ { -1.0f, -1.0f,  1.0f }, {.9f, .9f, .9f}, { 0.0f, 0.0f } },
			{ {  1.0f, -1.0f,  1.0f }, {.9f, .9f, .9f}, { 1.0f, 0.0f } },
			{ {  1.0f,  1.0f,  1.0f }, {.9f, .9f, .9f}, { 1.0f, 1.0f } },
			{ { -1.0f,  1.0f,  1.0f }, {.9f, .9f, .9f}, { 0.0f, 1.0f } },

			{ {  1.0f,  1.0f,  1.0f }, {.8f, .8f, .1f}, { 0.0f, 0.0f } },
			{ {  1.0f,  1.0f, -1.0f }, {.8f, .8f, .1f}, { 1.0f, 0.0f } },
			{ {  1.0f, -1.0f, -1.0f }, {.8f, .8f, .1f}, { 1.0f, 1.0f } },
			{ {  1.0f, -1.0f,  1.0f }, {.8f, .8f, .1f}, { 0.0f, 1.0f } },

			{ { -1.0f, -1.0f, -1.0f }, {.9f, .6f, .1f}, { 0.0f, 0.0f } },
			{ {  1.0f, -1.0f, -1.0f }, {.9f, .6f, .1f}, { 1.0f, 0.0f } },
			{ {  1.0f,  1.0f, -1.0f }, {.9f, .6f, .1f}, { 1.0f, 1.0f } },
			{ { -1.0f,  1.0f, -1.0f }, {.9f, .6f, .1f}, { 0.0f, 1.0f } },

			{ { -1.0f, -1.0f, -1.0f }, {.8f, .1f, .1f}, { 0.0f, 0.0f } },
			{ { -1.0f, -1.0f,  1.0f }, {.8f, .1f, .1f}, { 1.0f, 0.0f } },
			{ { -1.0f,  1.0f,  1.0f }, {.8f, .1f, .1f}, { 1.0f, 1.0f } },
			{ { -1.0f,  1.0f, -1.0f }, {.8f, .1f, .1f}, { 0.0f, 1.0f } },

			{ {  1.0f,  1.0f,  1.0f }, {.1f, .1f, .8f}, { 0.0f, 0.0f } },
			{ { -1.0f,  1.0f,  1.0f }, {.1f, .1f, .8f}, { 1.0f, 0.0f } },
			{ { -1.0f,  1.0f, -1.0f }, {.1f, .1f, .8f}, { 1.0f, 1.0f } },
			{ {  1.0f,  1.0f, -1.0f }, {.1f, .1f, .8f}, { 0.0f, 1.0f } },

			{ { -1.0f, -1.0f, -1.0f }, {.1f, .8f, .1f}, { 0.0f, 0.0f } },
			{ {  1.0f, -1.0f, -1.0f }, {.1f, .8f, .1f}, { 1.0f, 0.0f } },
			{ {  1.0f, -1.0f,  1.0f }, {.1f, .8f, .1f}, { 1.0f, 1.0f } },
			{ { -1.0f, -1.0f,  1.0f }, {.1f, .8f, .1f}, { 0.0f, 1.0f } },
		};
		indices = {
			0,1,2, 0,2,3, 4,5,6,  4,6,7, 8,9,10, 8,10,11, 12,13,14, 12,14,15, 16,17,18, 16,18,19, 20,21,22, 20,22,23
		};

		/*
		vertices = {




			// left face (white)
		  {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}, {0.0f, 0.0f}},
		  {{-.5f, .5f, .5f}, {.9f, .9f, .9f}, {1.0f, 1.0f}},
		  {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}, {0.0f, 1.0f}},
		  {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}, {1.0f, 0.0f}},

		  // right face (yellow)
		  {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}, {0.0f, 0.0f}},
		  {{.5f, .5f, .5f}, {.8f, .8f, .1f}, {1.0f, 1.0f}},
		  {{.5f, -.5f, .5f}, {.8f, .8f, .1f}, {0.0f, 1.0f}},
		  {{.5f, .5f, -.5f}, {.8f, .8f, .1f}, {1.0f, 0.0f}},

		  // top face (orange, remember y axis points down)
		  {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}, {0.0f, 0.0f}},
		  {{.5f, -.5f, .5f}, {.9f, .6f, .1f}, {1.0f, 1.0f}},
		  {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}, {0.0f, 1.0f}},
		  {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}, {1.0f, 0.0f}},

		  // bottom face (red)
		  {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}, {0.0f, 0.0f}},
		  {{.5f, .5f, .5f}, {.8f, .1f, .1f}, {1.0f, 1.0f}},
		  {{-.5f, .5f, .5f}, {.8f, .1f, .1f}, {0.0f, 1.0f}},
		  {{.5f, .5f, -.5f}, {.8f, .1f, .1f}, {1.0f, 0.0f}},

		  // nose face (blue)
		  {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}, {0.0f, 0.0f}},
		  {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}, {1.0f, 1.0f}},
		  {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}, {0.0f, 1.0f}},
		  {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}, {1.0f, 0.0f}},

		  // tail face (green)
		  {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}, {0.0f, 0.0f}},
		  {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}, {1.0f, 1.0f}},
		  {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}, {0.0f, 1.0f}},
		  {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}, {1.0f, 0.0f}},

		};


		indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
							12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

		*/
		
	}
}