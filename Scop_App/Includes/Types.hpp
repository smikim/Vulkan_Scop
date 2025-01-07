#pragma once

#include <vulkan/vulkan.h>
#include "../Library/Math/Matrix.hpp"

struct ShaderData {
	alignas(16) mymath::Mat4 modelMatrix;
	alignas(16) mymath::Mat4 viewMatrix;
	alignas(16)  mymath::Mat4 projectionMatrix;
	alignas(16) uint32_t colorMode;
};

// Uniform buffer block object
struct UniformBuffer {
	VkDeviceMemory memory = VK_NULL_HANDLE;
	VkBuffer buffer = VK_NULL_HANDLE;
	// The descriptor set stores the resources bound to the binding points in a shader
	// It connects the binding points of the different shaders with the buffers and images used for those bindings
	VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
	// We keep a pointer to the mapped buffer, so we can easily update it's contents via a memcpy
	uint8_t* mapped{ nullptr };
};