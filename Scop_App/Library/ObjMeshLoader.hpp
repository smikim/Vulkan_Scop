#pragma once

#include "vulkan/vulkan.hpp"
#include "../Includes/VulkanModel.hpp"

#include <vector>
#include <unordered_map>
#include <string>

#include "Math/Vector.hpp"
#include "Math/Matrix.hpp"

namespace vks
{
	class ObjMeshLoader
	{
	public:

		std::vector<vks::VulkanModel::Vertex> vertices;
		std::vector<uint32_t> indices;
		std::unordered_map<std::string, uint32_t> history;
		std::unordered_map<std::string, mymath::Vec3> colorLookup;
		mymath::Vec3 brushColor{-1.0f, -1.0f, -1.0f};
		mymath::Mat4 preTransform;

		std::vector<mymath::Vec3> v, vn;
		std::vector<mymath::Vec2> vt;
		std::vector<mymath::Vec3> vertexColors; // �߰��� ����
		

		float minX = std::numeric_limits<float>::max(), maxX = std::numeric_limits<float>::lowest();
		float minY = std::numeric_limits<float>::max(), maxY = std::numeric_limits<float>::lowest();
		float minZ = std::numeric_limits<float>::max(), maxZ = std::numeric_limits<float>::lowest();
		mymath::Vec3 sum{ 0.0f, 0.0f, 0.0f };

		
		ObjMeshLoader(const std::string& objFilepath);

		void	checkFile(const std::string& path, const std::string& extension);

		void	parseMtlFile(std::string& path);

		void read_vertex_data(const std::vector<std::string>& words);

		void read_texcoord_data(const std::vector<std::string>& words);

		void read_normal_data(const std::vector<std::string>& words);

		void read_face_data(const std::vector<std::string>& words);

		void read_corner(const std::string& vertex_description);

		std::vector<std::string> split(const std::string& line, const std::string& delimiter);

	};

}





