#pragma once

#include "vulkan/vulkan.h"
#include <vector>
#include <string>
#include "VulkanDevice.h"

#include "VulkanPipelineState.h"

namespace vks {

    class VulkanPipeline
    {
    public:

        VulkanPipeline(VulkanDevice& device,
            std::vector<VkPipelineShaderStageCreateInfo>& stage_create_infos,
            vks::VulkanPipelineState& pipelineState
        );

        virtual ~VulkanPipeline();

        VulkanPipeline(const VulkanPipeline&) = delete;
        VulkanPipeline& operator=(const VulkanPipeline&) = delete;

        void bind(VkCommandBuffer commandBuffer);

    private:
        VulkanPipeline();
        static std::vector<char> readFile(const std::string& filepath);

        VkShaderModule loadSPIRVShader(const std::string& filename);


        VulkanDevice& _vulkanDevice;
        VkPipeline _GraphicsPipeline;
 
    };

}

