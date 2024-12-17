#pragma once

#include "vulkan/vulkan.h"
#include <vector>

namespace vks
{
	struct VertexInputState
	{
		std::vector<VkVertexInputBindingDescription> bindings;

		std::vector<VkVertexInputAttributeDescription> attributes;
	};

	struct InputAssemblyState
	{
		VkPrimitiveTopology topology{ VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST };

		VkBool32 primitive_restart_enable{ VK_FALSE };
	};

	struct RasterizationState
	{
		VkBool32 depth_clamp_enable{ VK_FALSE };

		VkBool32 rasterizer_discard_enable{ VK_FALSE };

		VkPolygonMode polygon_mode{ VK_POLYGON_MODE_FILL };

		VkCullModeFlags cull_mode{ VK_CULL_MODE_BACK_BIT };

		VkFrontFace front_face{ VK_FRONT_FACE_COUNTER_CLOCKWISE };

		VkBool32 depth_bias_enable{ VK_FALSE };
	};

	struct ViewportState
	{
		uint32_t viewport_count{ 1 };

		uint32_t scissor_count{ 1 };
	};

	struct MultisampleState
	{
		VkSampleCountFlagBits rasterization_samples{ VK_SAMPLE_COUNT_1_BIT };

		VkBool32 sample_shading_enable{ VK_FALSE };

		float min_sample_shading{ 0.0f };

		VkSampleMask sample_mask{ 0 };

		VkBool32 alpha_to_coverage_enable{ VK_FALSE };

		VkBool32 alpha_to_one_enable{ VK_FALSE };
	};

	struct StencilOpState
	{
		VkStencilOp fail_op{ VK_STENCIL_OP_REPLACE };

		VkStencilOp pass_op{ VK_STENCIL_OP_REPLACE };

		VkStencilOp depth_fail_op{ VK_STENCIL_OP_REPLACE };

		VkCompareOp compare_op{ VK_COMPARE_OP_ALWAYS };
	};

	struct DepthStencilState
	{
		VkBool32 depth_test_enable{ VK_TRUE };

		VkBool32 depth_write_enable{ VK_TRUE };

		// Note: Using reversed depth-buffer for increased precision, so Greater depth values are kept
		VkCompareOp depth_compare_op{ VK_COMPARE_OP_GREATER };

		VkBool32 depth_bounds_test_enable{ VK_FALSE };

		VkBool32 stencil_test_enable{ VK_FALSE };

		StencilOpState front{};

		StencilOpState back{};
	};

	struct ColorBlendAttachmentState
	{
		VkBool32 blend_enable{ VK_FALSE };

		VkBlendFactor src_color_blend_factor{ VK_BLEND_FACTOR_ONE };

		VkBlendFactor dst_color_blend_factor{ VK_BLEND_FACTOR_ZERO };

		VkBlendOp color_blend_op{ VK_BLEND_OP_ADD };

		VkBlendFactor src_alpha_blend_factor{ VK_BLEND_FACTOR_ONE };

		VkBlendFactor dst_alpha_blend_factor{ VK_BLEND_FACTOR_ZERO };

		VkBlendOp alpha_blend_op{ VK_BLEND_OP_ADD };

		VkColorComponentFlags color_write_mask{ VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT };
	};

	struct ColorBlendState
	{
		VkBool32 logic_op_enable{ VK_FALSE };

		VkLogicOp logic_op{ VK_LOGIC_OP_CLEAR };

		std::vector<ColorBlendAttachmentState> attachments;
	};

	class VulkanPipelineState
	{


	public:
		void reset();

		void set_vertex_input_state(const VertexInputState& vertex_input_state);

		void set_input_assembly_state(const InputAssemblyState& input_assembly_state);

		void set_rasterization_state(const RasterizationState& rasterization_state);

		void set_viewport_state(const ViewportState& viewport_state);

		void set_multisample_state(const MultisampleState& multisample_state);

		void set_depth_stencil_state(const DepthStencilState& depth_stencil_state);

		void set_color_blend_state(const ColorBlendState& color_blend_state);

		void set_pipeline_layout(VkPipelineLayout pipeline_layout);

		void set_render_pass(VkRenderPass render_pass);

		const VertexInputState& get_vertex_input_state() const;

		const InputAssemblyState& get_input_assembly_state() const;

		const RasterizationState& get_rasterization_state() const;

		const ViewportState& get_viewport_state() const;

		const MultisampleState& get_multisample_state() const;

		const DepthStencilState& get_depth_stencil_state() const;

		const ColorBlendState& get_color_blend_state() const;

		const VkPipelineLayout get_pipeline_layout() const;

		const VkRenderPass get_render_pass() const;

	private:

		VertexInputState vertex_input_state{};

		InputAssemblyState input_assembly_state{};

		RasterizationState rasterization_state{};

		ViewportState viewport_state{};

		MultisampleState multisample_state{};

		DepthStencilState depth_stencil_state{};

		ColorBlendState color_blend_state{};

		VkPipelineLayout pipelineLayout;

		VkRenderPass render_pass;
	};

}



