#include "../Includes/VulkanPipeline.hpp"
#include "../Includes/VulkanTools.hpp"
#include "../Includes/helpers.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace vks {

	VulkanPipeline::VulkanPipeline(VulkanDevice& device, std::vector<VkPipelineShaderStageCreateInfo>& stage_create_infos, vks::VulkanPipelineState& pipelineState)
		: _vulkanDevice{ device }
	{
		VkGraphicsPipelineCreateInfo create_info;

		create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		create_info.stageCount = to_u32(stage_create_infos.size());
		create_info.pStages = stage_create_infos.data();
		create_info.pNext = nullptr;

		VkPipelineVertexInputStateCreateInfo vertex_input_state;

		vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertex_input_state.pVertexAttributeDescriptions = pipelineState.get_vertex_input_state().attributes.data();
		vertex_input_state.vertexAttributeDescriptionCount = to_u32(pipelineState.get_vertex_input_state().attributes.size());

		vertex_input_state.pVertexBindingDescriptions = pipelineState.get_vertex_input_state().bindings.data();
		vertex_input_state.vertexBindingDescriptionCount = to_u32(pipelineState.get_vertex_input_state().bindings.size());
		vertex_input_state.pNext = nullptr;
		vertex_input_state.flags = 0;

		VkPipelineInputAssemblyStateCreateInfo input_assembly_state;

		input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		input_assembly_state.topology = pipelineState.get_input_assembly_state().topology;
		input_assembly_state.primitiveRestartEnable = pipelineState.get_input_assembly_state().primitive_restart_enable;
		input_assembly_state.pNext = nullptr;
		input_assembly_state.flags = 0;

		VkPipelineViewportStateCreateInfo viewport_state;

		viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewport_state.viewportCount = pipelineState.get_viewport_state().viewport_count;
		viewport_state.scissorCount = pipelineState.get_viewport_state().scissor_count;
		viewport_state.pNext = nullptr;
		viewport_state.flags = 0;

		VkPipelineRasterizationStateCreateInfo rasterization_state;

		rasterization_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterization_state.depthClampEnable = pipelineState.get_rasterization_state().depth_clamp_enable;
		rasterization_state.rasterizerDiscardEnable = pipelineState.get_rasterization_state().rasterizer_discard_enable;
		rasterization_state.polygonMode = pipelineState.get_rasterization_state().polygon_mode;
		rasterization_state.cullMode = pipelineState.get_rasterization_state().cull_mode;
		rasterization_state.frontFace = pipelineState.get_rasterization_state().front_face;
		rasterization_state.depthBiasEnable = pipelineState.get_rasterization_state().depth_bias_enable;
		rasterization_state.depthBiasClamp = 1.0f;
		rasterization_state.depthBiasSlopeFactor = 1.0f;
		rasterization_state.lineWidth = 1.0f;
		rasterization_state.pNext = nullptr;
		rasterization_state.flags = 0;
		
		VkPipelineMultisampleStateCreateInfo multisample_state;

		multisample_state.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO; 
		multisample_state.sampleShadingEnable = pipelineState.get_multisample_state().sample_shading_enable;
		multisample_state.rasterizationSamples = pipelineState.get_multisample_state().rasterization_samples;
		multisample_state.minSampleShading = pipelineState.get_multisample_state().min_sample_shading;
		multisample_state.alphaToCoverageEnable = pipelineState.get_multisample_state().alpha_to_coverage_enable;
		multisample_state.alphaToOneEnable = pipelineState.get_multisample_state().alpha_to_one_enable;
		multisample_state.pNext = nullptr;
		multisample_state.flags = 0;
		multisample_state.pSampleMask = nullptr;

		if (pipelineState.get_multisample_state().sample_mask)
		{
			multisample_state.pSampleMask = &pipelineState.get_multisample_state().sample_mask;
		}

		VkPipelineDepthStencilStateCreateInfo depth_stencil_state;

		depth_stencil_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depth_stencil_state.depthTestEnable = pipelineState.get_depth_stencil_state().depth_test_enable;
		depth_stencil_state.depthWriteEnable = pipelineState.get_depth_stencil_state().depth_write_enable;
		depth_stencil_state.depthCompareOp = pipelineState.get_depth_stencil_state().depth_compare_op;
		depth_stencil_state.depthBoundsTestEnable = pipelineState.get_depth_stencil_state().depth_bounds_test_enable;
		depth_stencil_state.stencilTestEnable = pipelineState.get_depth_stencil_state().stencil_test_enable;
		depth_stencil_state.front.failOp = pipelineState.get_depth_stencil_state().front.fail_op;
		depth_stencil_state.front.passOp = pipelineState.get_depth_stencil_state().front.pass_op;
		depth_stencil_state.front.depthFailOp = pipelineState.get_depth_stencil_state().front.depth_fail_op;
		depth_stencil_state.front.compareOp = pipelineState.get_depth_stencil_state().front.compare_op;
		depth_stencil_state.front.compareMask = ~0U;
		depth_stencil_state.front.writeMask = ~0U;
		depth_stencil_state.front.reference = ~0U;
		depth_stencil_state.back.failOp = pipelineState.get_depth_stencil_state().back.fail_op;
		depth_stencil_state.back.passOp = pipelineState.get_depth_stencil_state().back.pass_op;
		depth_stencil_state.back.depthFailOp = pipelineState.get_depth_stencil_state().back.depth_fail_op;
		depth_stencil_state.back.compareOp = pipelineState.get_depth_stencil_state().back.compare_op;
		depth_stencil_state.back.compareMask = ~0U;
		depth_stencil_state.back.writeMask = ~0U;
		depth_stencil_state.back.reference = ~0U;
		depth_stencil_state.pNext = nullptr;
		depth_stencil_state.flags = 0;

		VkPipelineColorBlendStateCreateInfo color_blend_state;

		color_blend_state.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		color_blend_state.logicOpEnable = pipelineState.get_color_blend_state().logic_op_enable;
		color_blend_state.logicOp = pipelineState.get_color_blend_state().logic_op;
		color_blend_state.attachmentCount = to_u32(pipelineState.get_color_blend_state().attachments.size());
		color_blend_state.pAttachments = reinterpret_cast<const VkPipelineColorBlendAttachmentState*>(pipelineState.get_color_blend_state().attachments.data());
		color_blend_state.blendConstants[0] = 1.0f;
		color_blend_state.blendConstants[1] = 1.0f;
		color_blend_state.blendConstants[2] = 1.0f;
		color_blend_state.blendConstants[3] = 1.0f;
		color_blend_state.pNext = nullptr;
		color_blend_state.flags = 0;

		std::array<VkDynamicState, 9> dynamic_states{
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR,
			VK_DYNAMIC_STATE_LINE_WIDTH,
			VK_DYNAMIC_STATE_DEPTH_BIAS,
			VK_DYNAMIC_STATE_BLEND_CONSTANTS,
			VK_DYNAMIC_STATE_DEPTH_BOUNDS,
			VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
			VK_DYNAMIC_STATE_STENCIL_WRITE_MASK,
			VK_DYNAMIC_STATE_STENCIL_REFERENCE,
		};

		VkPipelineDynamicStateCreateInfo dynamic_state;

		dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamic_state.pDynamicStates = dynamic_states.data();
		dynamic_state.dynamicStateCount = to_u32(dynamic_states.size());
		dynamic_state.flags = 0;
		dynamic_state.pNext = nullptr;

		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.flags = 0;

		create_info.layout = pipelineState.get_pipeline_layout();
		create_info.renderPass = pipelineState.get_render_pass();
		create_info.subpass = 0;//create_info.subpass = pipelineState.get_subpass_index();

		auto result = vkCreateGraphicsPipelines(_vulkanDevice.getLogicalDevice(), nullptr, 1, &create_info, nullptr, &_GraphicsPipeline);

		if (result != VK_SUCCESS)
		{
			std::cerr << "Cannot create GraphicsPipelines" << std::endl;
			 throw std::runtime_error("Cannot create GraphicsPipelines");	
		}
	}

	VulkanPipeline::~VulkanPipeline()
	{
		vkDestroyPipeline(_vulkanDevice.getLogicalDevice(), _GraphicsPipeline, nullptr);
	}

	void VulkanPipeline::bind(VkCommandBuffer commandBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _GraphicsPipeline);
	}
	

	std::vector<char> VulkanPipeline::readFile(const std::string& filepath)
	{
		std::ifstream file{ filepath, std::ios::ate | std::ios::binary };

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file: " + filepath);
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;

	}


	// Vulkan loads its shaders from an immediate binary representation called SPIR-V
	// Shaders are compiled offline from e.g. GLSL using the reference glslang compiler
	// This function loads such a shader from a binary file and returns a shader module structure
	VkShaderModule VulkanPipeline::loadSPIRVShader(const std::string& filename)
	{
		size_t shaderSize;
		char* shaderCode{ nullptr };


		std::ifstream is(filename, std::ios::binary | std::ios::in | std::ios::ate);

		if (is.is_open())
		{
			shaderSize = is.tellg();
			is.seekg(0, std::ios::beg);
			// Copy file contents into a buffer
			shaderCode = new char[shaderSize];
			is.read(shaderCode, shaderSize);
			is.close();
			assert(shaderSize > 0);
		}

		if (shaderCode)
		{
			// Create a new shader module that will be used for pipeline creation
			VkShaderModuleCreateInfo shaderModuleCI{};
			shaderModuleCI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			shaderModuleCI.codeSize = shaderSize;
			shaderModuleCI.pCode = (uint32_t*)shaderCode;

			VkShaderModule shaderModule;
			VK_CHECK_RESULT(vkCreateShaderModule(_vulkanDevice.getLogicalDevice(), &shaderModuleCI, nullptr, &shaderModule));

			delete[] shaderCode;

			return shaderModule;
		}
		else
		{
			std::cerr << "Error: Could not open shader file \"" << filename << "\"" << std::endl;
			return VK_NULL_HANDLE;
		}
	}





}