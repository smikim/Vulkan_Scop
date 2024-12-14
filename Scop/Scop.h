#pragma once

#include "vulkan/vulkan.h"
#include "GlfwWindow.h"
#include "VulkanRenderer.h"

namespace scop
{
	class Scop
	{
	public:
		Scop();
		~Scop();

		//bool initVulkan();

		void run();
		void prepare();
		void render();

	private:
		
		uint32_t _width = 1280;
		uint32_t _height = 720;
		std::string _windowName = "Scop";
		GlfwWindow _window{ _width, _height, _windowName };
		
		vks::VulkanRenderer _renderer;

	};

}


