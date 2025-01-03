#include "ScopObject.h"
#include "Scop.h"
#include "ObjMeshLoader.h"

namespace scop
{
	/*glm::mat4 TransformComponent::mat4()
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

	}*/
	
	mymath::Mat4 TransformComponent::mat4()
	{
		const float c3 = glm::cos(rotation._z);
		const float s3 = glm::sin(rotation._z);
		const float c2 = glm::cos(rotation._x);
		const float s2 = glm::sin(rotation._x);
		const float c1 = glm::cos(rotation._y);
		const float s1 = glm::sin(rotation._y);

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

		/*glm::mat4 mat{ 1.0 };
		mat = glm::translate(mat, translation);
		mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		mat = glm::scale(mat, scale);

		return mat;*/
	}

	/*glm::mat3 TransformComponent::normalMatrix()
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
	}*/

	glm::mat3 TransformComponent::normalMatrix()
	{
		const float c3 = glm::cos(rotation._z);
		const float s3 = glm::sin(rotation._z);
		const float c2 = glm::cos(rotation._x);
		const float s2 = glm::sin(rotation._x);
		const float c1 = glm::cos(rotation._y);
		const float s1 = glm::sin(rotation._y);
		mymath::Vec3 invScale;
		invScale._x = 1.0f / scale._x;
		invScale._y = 1.0f / scale._y;
		invScale._z = 1.0f / scale._z;
	
		return glm::mat3{
			{
				invScale._x * (c1 * c3 + s1 * s2 * s3),
				invScale._x * (c2 * s3),
				invScale._x * (c1 * s2 * s3 - c3 * s1),
			},
			{
				invScale._y * (c3 * s1 * s2 - c1 * s3),
				invScale._y * (c2 * c3),
				invScale._y * (c1 * c3 * s2 + s1 * s3),
			},
			{
				invScale._z * (c2 * s1),
				invScale._z * (-s2),
				invScale._z * (c1 * c2),
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

	vks::VulkanModel* ScopObject::CreateObjMeshObject(std::string& filename)
	{
		vks::VulkanDevice* vulkanDevice = _renderer->getVulkanDevice();

		try
		{
			glm::mat4 flipYMatrix = glm::mat4(1.0f);
			flipYMatrix[1][1] = -1.0f;

			ObjMeshLoader objLoader{ filename, glm::mat4(1.0f) };

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

		// Add creatMeshObject()
		// call obj loader
		//_vulkanModel = CreateBoxMeshObject();
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
		//UpdateTransform();
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