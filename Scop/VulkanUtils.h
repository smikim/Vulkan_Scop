#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "VulkanDevice.h"
#include "VulkanTools.h"

namespace utils
{
	// Vulkan loads its shaders from an immediate binary representation called SPIR-V
	// Shaders are compiled offline from e.g. GLSL using the reference glslang compiler
	// This function loads such a shader from a binary file and returns a shader module structure
	VkShaderModule loadSPIRVShader(const std::string& filename, vks::VulkanDevice& vulkanDevice);

}
