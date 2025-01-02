#pragma once

#include "vulkan/vulkan.h"
#include "VulkanModel.h"

#include <vector>
#include <unordered_map>
#include <string>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace scop
{
	class ObjMeshLoader
	{
	public:

		std::vector<vks::VulkanModel::Vertex> vertices;
		std::vector<uint32_t> indices;
		std::unordered_map<std::string, uint32_t> history;
		std::unordered_map<std::string, glm::vec3> colorLookup;
		glm::vec3 brushColor;
		glm::mat4 preTransform;

		std::vector<glm::vec3> v, vn;
		std::vector<glm::vec2> vt;
		std::vector<glm::vec3> vertexColors; // √ﬂ∞°µ» ∫§≈Õ

		float minX = std::numeric_limits<float>::max(), maxX = std::numeric_limits<float>::lowest();
		float minY = std::numeric_limits<float>::max(), maxY = std::numeric_limits<float>::lowest();
		float minZ = std::numeric_limits<float>::max(), maxZ = std::numeric_limits<float>::lowest();

		ObjMeshLoader(const std::string& objFilepath, glm::mat4 preTransform);

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





