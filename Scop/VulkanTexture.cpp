#include "VulkanTexture.h"
#include "BmpLoader.h"
#include "tmpLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
// 이미지 로드 전에 수직 반전 설정


namespace vks
{
	VulkanTexture::VulkanTexture(std::string& filename, VkQueue copyQueue, vks::VulkanDevice* vulkanDevice)
		: _filename(filename), _vulkanDevice(vulkanDevice)
	{
		createTextureImage(copyQueue, vulkanDevice);
		createTextureImageView();
		createTextureSampler();
	}

	VulkanTexture::~VulkanTexture()
	{
		destroy();
	}

	void VulkanTexture::createTextureImage(VkQueue copyQueue, vks::VulkanDevice* vulkanDevice)
	{
		//BMP my_loader = BMP(_filename.c_str());

		int texWidth, texHeight, texChannels;
		//stbi_set_flip_vertically_on_load(true);
		//stbi_uc* pixels = stbi_load(_filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		
		BmpLoader bmpLoader;
		std::vector<uint8_t> imageData;
		bmpLoader.readFile(_filename);
		bmpLoader.processImage(imageData);

		texWidth = bmpLoader.infoHeader.btWidth;
		texHeight = bmpLoader.infoHeader.btHeight;

		//texWidth = my_loader.info_header.width;
		//texHeight = my_loader.info_header.height;

		VkDeviceSize imageSize = texWidth * texHeight * 4;
		//VkDeviceSize imageSize = bmpLoader.infoHeader.btWidth * bmpLoader.infoHeader.btHeight * 4;

		/*if (!pixels) {
			throw std::runtime_error("failed to load texture image!");
		}*/

		/*if (!bmpLoader.data) {
			throw std::runtime_error("failed to load texture image!");
		}*/

		this->_vulkanDevice = vulkanDevice;

		// Use a separate command buffer for texture loading
		VkCommandBuffer copyCmd = vulkanDevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = imageSize;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(vulkanDevice->getLogicalDevice(), &bufferInfo, nullptr, &stagingBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(vulkanDevice->getLogicalDevice(), stagingBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = vulkanDevice->get_gpu().getMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);


		if (vkAllocateMemory(vulkanDevice->getLogicalDevice(), &allocInfo, nullptr, &stagingBufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(vulkanDevice->getLogicalDevice(), stagingBuffer, stagingBufferMemory, 0);

		void* data;
		vkMapMemory(vulkanDevice->getLogicalDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
		//memcpy(data, pixels, static_cast<size_t>(imageSize));
		// TODO
		memcpy(data, imageData.data(), imageData.size());
		//memcpy(data, bmpLoader.data.get(), static_cast<size_t>(imageSize));
		
		vkUnmapMemory(vulkanDevice->getLogicalDevice(), stagingBufferMemory);

		//stbi_image_free(pixels);

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = static_cast<uint32_t>(texWidth);
		//imageInfo.extent.width = static_cast<uint32_t>(bmpLoader.infoHeader.btWidth);
		imageInfo.extent.height = static_cast<uint32_t>(texHeight);
		//imageInfo.extent.height = static_cast<uint32_t>(bmpLoader.infoHeader.btHeight);
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0; // Optional

		if (vkCreateImage(vulkanDevice->getLogicalDevice(), &imageInfo, nullptr, &_TextureImage) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image!");
		}

		//VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(vulkanDevice->getLogicalDevice(), _TextureImage, &memRequirements);

		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = vulkanDevice->get_gpu().getMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);


		if (vkAllocateMemory(vulkanDevice->getLogicalDevice(), &allocInfo, nullptr, &_TextureImageMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(vulkanDevice->getLogicalDevice(), _TextureImage, _TextureImageMemory, 0);

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			static_cast<uint32_t>(texWidth),
			//static_cast<uint32_t>(bmpLoader.infoHeader.btWidth),
			static_cast<uint32_t>(texHeight),
			//static_cast<uint32_t>(bmpLoader.infoHeader.btHeight),
			1
		};


		// Image barrier for optimal image (target)
		// Optimal image will be used as destination for the copy
		vks::tools::setImageLayout(
			copyCmd,
			_TextureImage,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			subresourceRange);

		// Copy mip levels from staging buffer
		vkCmdCopyBufferToImage(
			copyCmd,
			stagingBuffer,
			_TextureImage,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

		vks::tools::setImageLayout(
			copyCmd,
			_TextureImage,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			subresourceRange);

		vulkanDevice->flushCommandBuffer(copyCmd, copyQueue);

		// Clean up staging resources
		vkDestroyBuffer(vulkanDevice->getLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(vulkanDevice->getLogicalDevice(), stagingBufferMemory, nullptr);
	}

	void VulkanTexture::destroy()
	{
		vkDestroySampler(_vulkanDevice->getLogicalDevice(), _TextureSampler, nullptr);
		vkDestroyImageView(_vulkanDevice->getLogicalDevice(), _TextureImageView, nullptr);
		vkDestroyImage(_vulkanDevice->getLogicalDevice(), _TextureImage, nullptr);
		vkFreeMemory(_vulkanDevice->getLogicalDevice(), _TextureImageMemory, nullptr);
	}

	void VulkanTexture::createTextureImageView()
	{
		_TextureImageView = createImageView(_TextureImage, VK_FORMAT_R8G8B8A8_SRGB);
	}

	VkImageView VulkanTexture::createImageView(VkImage image, VkFormat format)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(_vulkanDevice->getLogicalDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture image view!");
		}

		return imageView;
	}

	void VulkanTexture::createTextureSampler()
	{
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		//samplerInfo.anisotropyEnable = VK_TRUE;
		//VkPhysicalDeviceProperties properties{};
		//vkGetPhysicalDeviceProperties(vulkanDevice->get_gpu().get_physical_device(), &properties);
		//samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		// Enable anisotropic filtering
		// This feature is optional, so we must check if it's supported on the device
		if (_vulkanDevice->get_gpu().get_device_features().samplerAnisotropy) {
			// Use max. level of anisotropy for this example
			samplerInfo.maxAnisotropy = _vulkanDevice->get_gpu().get_device_properties().limits.maxSamplerAnisotropy;
			samplerInfo.anisotropyEnable = VK_TRUE;
		}
		else {
			// The device does not support anisotropic filtering
			samplerInfo.maxAnisotropy = 1.0;
			samplerInfo.anisotropyEnable = VK_FALSE;
		}
		if (vkCreateSampler(_vulkanDevice->getLogicalDevice(), &samplerInfo, nullptr, &_TextureSampler) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture sampler!");
		}
	}
}