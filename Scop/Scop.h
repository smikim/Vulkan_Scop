#pragma once

#include "vulkan/vulkan.h"
#include "VulkanRenderer.h"
#include "ScopObject.h"

class GlfwWindow;

#define MAX_FRAME_TIME 60

namespace scop
{
	class Scop
	{
	public:
		Scop();
		~Scop();

		void run();
		void prepare(std::string& ObjFilename, std::string& BmpFilename);
		void update();
		void render();
		ScopObject* createScopObject(std::string& ObjFilename, std::string& BmpFilename);

	private:
		
		uint32_t _width = 1280;
		uint32_t _height = 720;
		std::string _windowName = "Scop";
		
		GlfwWindow _window{ _width, _height, _windowName };
		
		vks::VulkanRenderer _renderer{ _window };
		std::vector<ScopObject*> ScopObjects;

		void deleteScopObjects();

	public: 
		vks::VulkanRenderer* getVulkanRenderer() { return &_renderer; }
	};

}


