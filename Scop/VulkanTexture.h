#pragma once
#include "VulkanDevice.h"
#include <string>

namespace vks
{
	class VulkanTexture
	{
	public:
		VulkanTexture(std::string& filename, VkQueue copyQueue, vks::VulkanDevice* vulkanDevice);
		~VulkanTexture();
		void createTextureImage(VkQueue copyQueue, vks::VulkanDevice* vulkanDevice);
		void destroy();
		void createTextureImageView();
		VkImageView createImageView(VkImage image, VkFormat format);
		void createTextureSampler();

		std::string _filename;
		vks::VulkanDevice* _vulkanDevice;
		VkImage _TextureImage;
		VkDeviceMemory _TextureImageMemory;
		VkImageView _TextureImageView;
		VkSampler _TextureSampler;
	};

}

