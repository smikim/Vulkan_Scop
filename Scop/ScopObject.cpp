#include "ScopObject.h"
#include "Scop.h"
#include "ObjMeshLoader.h"

namespace scop
{
	mymath::Mat4 TransformComponent::mat4()
	{
		const float c3 = std::cos(rotation._z);
		const float s3 = std::sin(rotation._z);
		const float c2 = std::cos(rotation._x);
		const float s2 = std::sin(rotation._x);
		const float c1 = std::cos(rotation._y);
		const float s1 = std::sin(rotation._y);

		mymath::Mat4 res{ 1.0 };
		res[0] = scale._x * (c1 * c3 + s1 * s2 * s3);
		res[1] = scale._x * (c2 * s3);
		res[2] = scale._x * (c1 * s2 * s3 - c3 * s1);
		res[3] = 0.0f;

		res[4] = scale._y * (c3 * s1 * s2 - c1 * s3);
		res[5] = scale._y * (c2 * c3);
		res[6] = scale._y * (c1 * c3 * s2 + s1 * s3);
		res[7] = 0.0f;

		res[8] = scale._z * (c2 * s1);
		res[9] = scale._z * (-s2);
		res[10] = scale._z * (c1 * c2);
		res[11] = 0.0f;

		res[12] = translation._x;
		res[13] = translation._y;
		res[14] = translation._z;
		res[15] = 1.0f;

		return res;
	}


	mymath::Mat4 TransformComponent::getWorldMatrix()
	{
		return worldMatrix;
	}

	mymath::Mat4 TransformComponent::normalMatrix()
	{
		const float c3 = std::cos(rotation._z);
		const float s3 = std::sin(rotation._z);
		const float c2 = std::cos(rotation._x);
		const float s2 = std::sin(rotation._x);
		const float c1 = std::cos(rotation._y);
		const float s1 = std::sin(rotation._y);

		mymath::Vec3 invScale;
		invScale._x = 1.0f / scale._x;
		invScale._y = 1.0f / scale._y;
		invScale._z = 1.0f / scale._z;
	
		mymath::Mat4 res{ 1.0 };

		res[0] = invScale._x * (c1 * c3 + s1 * s2 * s3);
		res[1] = invScale._x * (c2 * s3);
		res[2] = invScale._x * (c1 * s2 * s3 - c3 * s1);
		res[3] = 0.0f;

		res[4] = invScale._y * (c3 * s1 * s2 - c1 * s3);
		res[5] = invScale._y * (c2 * c3);
		res[6] = invScale._y * (c1 * c3 * s2 + s1 * s3);
		res[7] = 0.0f;

		res[8] = invScale._z * (c2 * s1);
		res[9] = invScale._z * (-s2);
		res[10] = invScale._z * (c1 * c2);
		res[11] = 0.0f;

		res[12] = 0;
		res[13] = 0;
		res[14] = 0;
		res[15] = 1.0f;

		return res;
	}

	vks::IVulkanModel* ScopObject::CreateBoxMeshObject()
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

	vks::IVulkanModel* ScopObject::CreateObjMeshObject(std::string& filename)
	{
		vks::VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();

		try
		{
			ObjMeshLoader objLoader{ filename };

			_vulkanModel = _renderer->CreateBasicMeshObject();

			_renderer->BeginCreateMesh(_vulkanModel, objLoader.vertices);
			_renderer->InsertIndexBuffer(_vulkanModel, objLoader.indices);
			_renderer->EndCreateMesh(_vulkanModel);

		}
		catch (const std::exception& e)
		{
			std::cerr << "Failed to load OBJ mesh: " << e.what() << std::endl;
			return nullptr;
		}
		
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
	
	bool ScopObject::Initialize(Scop* scop, std::string& filename)
	{
		_scop = scop;
		_renderer = scop->getVulkanRenderer();

		_vulkanModel = CreateObjMeshObject(filename);

		if (_vulkanModel == nullptr)
			return false;
		return true;
	}

	void ScopObject::setTranslation(float x, float y, float z)
	{
		_transform.translation._x = x;
		_transform.translation._y = y;
		_transform.translation._z = z;

		_transform.matTrans = mymath::translate(_transform.matTrans, _transform.translation);
	}

	void ScopObject::setScale(float x, float y, float z)
	{
		_transform.scale._x = x;
		_transform.scale._y = y;
		_transform.scale._z = z;
		
		_transform.matScale = mymath::scale(_transform.matScale, _transform.scale);
	}

	void ScopObject::setRotation(float x, float y, float z)
	{
		_transform.rotation._x = x;
		_transform.rotation._y = y;
		_transform.rotation._z = z;

		_transform.matRot = mymath::rotate(_transform.matRot, glm::radians(x), mymath::Vec3(1.0f, 0.0f, 0.0f));
		_transform.matRot = mymath::rotate(_transform.matRot, glm::radians(y), mymath::Vec3(0.0f, 1.0f, 0.0f));
		_transform.matRot = mymath::rotate(_transform.matRot, glm::radians(z), mymath::Vec3(0.0f, 0.0f, 1.0f));

	}

	void ScopObject::moveRotation(float x, float y, float z)
	{
		_transform.rotation._x += x;
		_transform.rotation._y += y;
		_transform.rotation._z += z;

		_transform.matRot = mymath::rotate(_transform.matRot, glm::radians(x), mymath::Vec3(1.0f, 0.0f, 0.0f));
		_transform.matRot = mymath::rotate(_transform.matRot, glm::radians(y), mymath::Vec3(0.0f, 1.0f, 0.0f));
		_transform.matRot = mymath::rotate(_transform.matRot, glm::radians(z), mymath::Vec3(0.0f, 0.0f, 1.0f));

	}

	void ScopObject::moveTranslation(float x, float y, float z)
	{
		_transform.translation._x += x;
		_transform.translation._y += y;
		_transform.translation._z += z;

		_transform.matTrans = mymath::translate(_transform.matTrans, _transform.translation);


	}

	void ScopObject::Run()
	{
		_renderer->updateObjectUniformBuffer(_vulkanModel, _transform.mat4(), _colorMode);
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

		
	}
}