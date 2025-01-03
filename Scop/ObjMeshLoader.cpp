#include "ObjMeshLoader.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace scop {

	ObjMeshLoader::ObjMeshLoader(const std::string& objFilepath)
	{
		this->preTransform = preTransform;

		if (objFilepath.empty())
		{
			throw std::runtime_error(std::string("filename is wrong"));

		}

		std::string mtlFilename;

		std::ifstream file;
		// 확장자 체크 .obj
		checkFile(objFilepath, ".obj");

		file.open(objFilepath);
		if (!file.is_open()) {
			throw std::runtime_error(std::string("Failed to open OBJ file: ") + objFilepath);
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		if (file.fail()) {
			file.close();
			throw std::runtime_error(std::string("Failed to read OBJ file: ") + objFilepath);
		}
		file.close();

		std::string line;
		std::vector<std::string> words;

		while (std::getline(buffer, line)) {
			std::string delimiter = " ";
			words = split(line, delimiter);

			if (!words[0].compare("mtllib")) {
				// save mtl
				mtlFilename = words[1];
				std::cout << "MTL file name : " << mtlFilename << std::endl;
				parseMtlFile(mtlFilename);
			}

			if (!words[0].compare("v")) {
				read_vertex_data(words);
			}
			if (!words[0].compare("vt")) {
				read_texcoord_data(words);
			}
			if (!words[0].compare("vn")) {
				read_normal_data(words);
			}
			if (!words[0].compare("usemtl")) {
				std::cout << words[1] << std::endl;
				if (colorLookup.contains(words[1])) {
					brushColor = colorLookup[words[1]];
				}
				else {
					brushColor = mymath::Vec3(-1.0f);
				}
			}

			if (!words[0].compare("f")) {
				read_face_data(words);
			}
		}
		file.close();

		// 중심축 이동 
		// save triangle id
		int vertexCount = vertices.size();
		mymath::Vec3 center = sum / (float)vertexCount;

		uint32_t triangleID = 0;
		for (auto& vertex : vertices) {
			vertex.position[0] -= center._x;
			vertex.position[1] -= center._y;
			vertex.position[2] -= center._z;

			vertex.triangleID = triangleID++;
		}

		//glm::vec2 texture_coords[4] = {
		//	{ 0.0f, 0.0f },
		//	{ 1.0f, 0.0f },
		//	{ 1.0f, 1.0f },
		//	{ 0.0f, 1.0f }
		//};

		//std::size_t	i = 0;
		//for (auto& triangle : triangles) {
		//	if (i % 2 == 0) {
		//		vertices[triangle.indices[0]].uv[0] = texture_coords[0].x;
		//		vertices[triangle.indices[0]].uv[1] = texture_coords[0].y;

		//		vertices[triangle.indices[1]].uv[0] = texture_coords[1].x;
		//		vertices[triangle.indices[1]].uv[1] = texture_coords[1].y;

		//		vertices[triangle.indices[2]].uv[0] = texture_coords[2].x;
		//		vertices[triangle.indices[2]].uv[1] = texture_coords[2].y;


		//		//triangle.indices[1].texture = 1;
		//		
		//		//triangle.indices[2].texture = 2;
		//	}
		//	else {

		//		vertices[triangle.indices[0]].uv[0] = texture_coords[0].x;
		//		vertices[triangle.indices[0]].uv[1] = texture_coords[0].y;

		//		vertices[triangle.indices[1]].uv[0] = texture_coords[2].x;
		//		vertices[triangle.indices[1]].uv[1] = texture_coords[2].y;

		//		vertices[triangle.indices[2]].uv[0] = texture_coords[3].x;
		//		vertices[triangle.indices[2]].uv[1] = texture_coords[3].y;
		//		
		//		

		//		//triangle.indices[0].texture = 0;
		//		//triangle.indices[1].texture = 2;
		//		//triangle.indices[2].texture = 3;
		//	}
		//	i++;
		//}

		
	}

	void ObjMeshLoader::checkFile(const std::string& path, const std::string& extension)
	{
		/*	if (path.empty()) {
				throw std::invalid_argument("Empty file name");
			}*/
		std::size_t	extension_pos = path.rfind('.');

		if (extension_pos == std::string::npos) {
			throw std::invalid_argument("File '" + path + "' has no extension");
		}
		else if (path.substr(extension_pos) != extension) {
			throw std::invalid_argument("File '" + path + "' is not a ." + extension + "file");
		}
	}

	void ObjMeshLoader::parseMtlFile(std::string& mtlFilename)
	{
		if (!mtlFilename.empty())
		{
			// 확장자 체크 .mtl
			checkFile(mtlFilename, ".mtl");

			mtlFilename = "models/" + mtlFilename;
			std::ifstream file;
			file.open(mtlFilename);
			if (!file.is_open()) {
				return;
				//throw std::runtime_error(std::string("Failed to open material file: ") + mtlFilename);
			}

			std::stringstream buffer;
			buffer << file.rdbuf();
			if (file.fail()) {
				throw std::runtime_error(std::string("Failed to read material file: ") + mtlFilename);

			}
			file.close();

			std::string line;
			std::string materialName;
			std::vector<std::string> words;

			while (std::getline(buffer, line)) {

				std::string delimiter = " ";
				words = split(line, delimiter);

				if (!words[0].compare("newmtl")) {
					materialName = words[1];
				}
				if (!words[0].compare("Kd")) {
					brushColor = mymath::Vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
					colorLookup.insert({ materialName, brushColor });
				}
			}


		}
	}

	void ObjMeshLoader::read_vertex_data(const std::vector<std::string>& words) {
		mymath::Vec3 new_vertex = mymath::Vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
		v.push_back(new_vertex);

		minX = std::min(new_vertex._x, minX);
		minY = std::min(new_vertex._y, minY);
		minZ = std::min(new_vertex._z, minZ);

		maxX = std::max(new_vertex._x, maxX);
		maxY = std::max(new_vertex._y, maxY);
		maxZ = std::max(new_vertex._z, maxZ);

		sum += new_vertex;

		// 색상 값이 포함된 경우
		if (words.size() > 6) {
			mymath::Vec3 color = mymath::Vec3(std::stof(words[4]), std::stof(words[5]), std::stof(words[6]));
			vertexColors.push_back(color);
		}
		else {
			vertexColors.push_back(mymath::Vec3(-1.0f)); // 기본 색상
		}
	}

	void ObjMeshLoader::read_texcoord_data(const std::vector<std::string>& words) {
		mymath::Vec2 new_texcoord = mymath::Vec2(std::stof(words[1]), std::stof(words[2]));
		vt.push_back(new_texcoord);
	}

	void ObjMeshLoader::read_normal_data(const std::vector<std::string>& words) {
		mymath::Vec3 new_normal = mymath::Vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
		vn.push_back(new_normal);
	}

	void ObjMeshLoader::read_face_data(const std::vector<std::string>& words) {

		size_t triangleCount = words.size() - 3;

		mymath::Vec2 texture_coords[4] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};
		// 0, 1, 2, 0, 2, 3, 0, 3, 4 
		//uint32_t	 triangleID = 0;
		for (int i = 0; i < triangleCount; ++i) {
			
			//if((i % 2) == 0) {
			////if(i == 0) {
			//	// 0, 1, 2
			//	read_corner(words[1], texture_coords[0]);
			//	read_corner(words[2 + i], texture_coords[1]);
			//	read_corner(words[3 + i], texture_coords[2]);
			//}
			//else
			//{
			//	// 0, 2, 3
			//	// i = 1, 2, 3, 4, 5, ...
			//	// 0, ( 2 + i) - (i), (3 + i) - (i)
			//	read_corner(words[1], texture_coords[0]);
			//	read_corner(words[2 + i], texture_coords[2]);
			//	read_corner(words[3 + i], texture_coords[3]);
			//}

			
			
			read_corner(words[1]);
			read_corner(words[2 + i]);
			read_corner(words[3 + i]);
		
			
		}
	}

	//void ObjMeshLoader::read_corner(const std::string& vertex_description, glm::vec2& tex) {
	void ObjMeshLoader::read_corner(const std::string& vertex_description) {
		auto it = history.find(vertex_description);
		if (it != history.end()) {
			indices.push_back(it->second);
			return;
		}

		uint32_t index = static_cast<uint32_t>(history.size());
		history.insert({ vertex_description, index });
		indices.push_back(index);
		//triangle = index;

		std::vector<std::string> v_vt_vn = split(vertex_description, "/");

		vks::VulkanModel::Vertex vertex{};

		//position
		mymath::Vec3 pos = v[std::stol(v_vt_vn[0]) - 1];
		vertex.position[0] = pos._x;
		vertex.position[1] = pos._y;
		vertex.position[2] = pos._z;


		// color
		mymath::Vec3 color = vertexColors[std::stol(v_vt_vn[0]) - 1];
		if (color == mymath::Vec3(-1.0f)) { // 기본 색상인 경우 무지개색 사용

			// color
			if (brushColor == mymath::Vec3(-1.0f)) { // 기본 색상인 경우 무지개색 사용
				static const mymath::Vec3 rainbowColors[] = {
					{1.0f, 0.0f, 0.0f}, // 빨강
					{1.0f, 0.5f, 0.0f}, // 주황
					{1.0f, 1.0f, 0.0f}, // 노랑
					{0.0f, 1.0f, 0.0f}, // 초록
					{0.0f, 0.0f, 1.0f}, // 파랑
					{0.29f, 0.0f, 0.51f}, // 남색
					{0.56f, 0.0f, 1.0f}  // 보라
				};
				size_t colorIndex = index % (sizeof(rainbowColors) / sizeof(rainbowColors[0]));
				vertex.color[0] = rainbowColors[colorIndex]._x;
				vertex.color[1] = rainbowColors[colorIndex]._y;
				vertex.color[2] = rainbowColors[colorIndex]._z;
			}
			else {
				vertex.color[0] = brushColor._x;
				vertex.color[1] = brushColor._y;
				vertex.color[2] = brushColor._z;
			}


			//vertex.color[0] = 1.0f;
			//vertex.color[1] = 1.0f;
			//vertex.color[2] = 1.0f;

			//static const glm::vec3 rainbowColors[] = {
			//	{1.0f, 0.0f, 0.0f}, // 빨강
			//	{1.0f, 0.5f, 0.0f}, // 주황
			//	{1.0f, 1.0f, 0.0f}, // 노랑
			//	{0.0f, 1.0f, 0.0f}, // 초록
			//	{0.0f, 0.0f, 1.0f}, // 파랑
			//	{0.29f, 0.0f, 0.51f}, // 남색
			//	{0.56f, 0.0f, 1.0f}  // 보라
			//};
			//size_t colorIndex = index % (sizeof(rainbowColors) / sizeof(rainbowColors[0]));
			//vertex.color[0] = rainbowColors[colorIndex].r;
			//vertex.color[1] = rainbowColors[colorIndex].g;
			//vertex.color[2] = rainbowColors[colorIndex].b;

		}
		else {
			vertex.color[0] = color._x;
			vertex.color[1] = color._y;
			vertex.color[2] = color._z;
		}

		
		
		

		//texcoord
		mymath::Vec2 texcoord = mymath::Vec2(0.0f, 0.0f);
		if (v_vt_vn.size() > 1 && !v_vt_vn[1].empty()) {
			texcoord = vt[std::stol(v_vt_vn[1]) - 1];
		}
		if (!vt.empty())
		{
			vertex.uv[0] = texcoord._x;
			vertex.uv[1] = texcoord._y;
		}
		else
		{
			vertex.uv[0] = (vertex.position[0] - minX) / (maxX - minX);
			
			//vertex.uv[0] = tex.x;
			
			vertex.uv[1] = 1.0f - ((vertex.position[1] - minY) / (maxY - minY));
			//vertex.uv[1] = tex.y;


		}
			


		//normal
		mymath::Vec3 normal = mymath::Vec3(0.0f, 0.0f, 0.0f); // 기본값 설정
		if (v_vt_vn.size() > 2 && !v_vt_vn[2].empty()) {
			normal = vn[std::stol(v_vt_vn[2]) - 1];
		}
		vertex.normal[0] = normal._x;
		vertex.normal[1] = normal._y;
		vertex.normal[2] = normal._z;

		
		vertices.push_back(vertex);
	}

	std::vector<std::string> ObjMeshLoader::split(const std::string& line, const std::string& delimiter)
	{
		std::vector<std::string> split_line;
		size_t start = 0;
		size_t end = line.find(delimiter);

		while (end != std::string::npos) {
			if (end != start) { // 연속된 구분자를 무시
				split_line.push_back(line.substr(start, end - start));
			}
			//split_line.push_back(line.substr(start, end - start));
			start = end + delimiter.length();
			end = line.find(delimiter, start);
		}
		split_line.push_back(line.substr(start));

		return split_line;
	}
}

