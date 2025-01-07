#include "../Includes/VulkanPipelineState.hpp"
#include <tuple>    

bool operator==(const VkVertexInputAttributeDescription& lhs, const VkVertexInputAttributeDescription& rhs)
{
	return std::tie(lhs.binding, lhs.format, lhs.location, lhs.offset) == std::tie(rhs.binding, rhs.format, rhs.location, rhs.offset);
}

bool operator==(const VkVertexInputBindingDescription& lhs, const VkVertexInputBindingDescription& rhs)
{
	return std::tie(lhs.binding, lhs.inputRate, lhs.stride) == std::tie(rhs.binding, rhs.inputRate, rhs.stride);
}

bool operator==(const vks::ColorBlendAttachmentState& lhs, const vks::ColorBlendAttachmentState& rhs)
{
	return std::tie(lhs.alpha_blend_op, lhs.blend_enable, lhs.color_blend_op, lhs.color_write_mask, lhs.dst_alpha_blend_factor, lhs.dst_color_blend_factor, lhs.src_alpha_blend_factor, lhs.src_color_blend_factor) ==
		std::tie(rhs.alpha_blend_op, rhs.blend_enable, rhs.color_blend_op, rhs.color_write_mask, rhs.dst_alpha_blend_factor, rhs.dst_color_blend_factor, rhs.src_alpha_blend_factor, rhs.src_color_blend_factor);
}

bool operator!=(const vks::StencilOpState& lhs, const vks::StencilOpState& rhs)
{
	return std::tie(lhs.compare_op, lhs.depth_fail_op, lhs.fail_op, lhs.pass_op) != std::tie(rhs.compare_op, rhs.depth_fail_op, rhs.fail_op, rhs.pass_op);
}

bool operator!=(const vks::VertexInputState& lhs, const vks::VertexInputState& rhs)
{
	return lhs.attributes != rhs.attributes || lhs.bindings != rhs.bindings;
}

bool operator!=(const vks::InputAssemblyState& lhs, const vks::InputAssemblyState& rhs)
{
	return std::tie(lhs.primitive_restart_enable, lhs.topology) != std::tie(rhs.primitive_restart_enable, rhs.topology);
}

bool operator!=(const vks::RasterizationState& lhs, const vks::RasterizationState& rhs)
{
	return std::tie(lhs.cull_mode, lhs.depth_bias_enable, lhs.depth_clamp_enable, lhs.front_face, lhs.front_face, lhs.polygon_mode, lhs.rasterizer_discard_enable) !=
		std::tie(rhs.cull_mode, rhs.depth_bias_enable, rhs.depth_clamp_enable, rhs.front_face, rhs.front_face, rhs.polygon_mode, rhs.rasterizer_discard_enable);
}

bool operator!=(const vks::ViewportState& lhs, const vks::ViewportState& rhs)
{
	return lhs.viewport_count != rhs.viewport_count || lhs.scissor_count != rhs.scissor_count;
}

bool operator!=(const vks::MultisampleState& lhs, const vks::MultisampleState& rhs)
{
	return std::tie(lhs.alpha_to_coverage_enable, lhs.alpha_to_one_enable, lhs.min_sample_shading, lhs.rasterization_samples, lhs.sample_mask, lhs.sample_shading_enable) !=
		std::tie(rhs.alpha_to_coverage_enable, rhs.alpha_to_one_enable, rhs.min_sample_shading, rhs.rasterization_samples, rhs.sample_mask, rhs.sample_shading_enable);
}

bool operator!=(const vks::DepthStencilState& lhs, const vks::DepthStencilState& rhs)
{
	return std::tie(lhs.depth_bounds_test_enable, lhs.depth_compare_op, lhs.depth_test_enable, lhs.depth_write_enable, lhs.stencil_test_enable) !=
		std::tie(rhs.depth_bounds_test_enable, rhs.depth_compare_op, rhs.depth_test_enable, rhs.depth_write_enable, rhs.stencil_test_enable) ||
		lhs.back != rhs.back || lhs.front != rhs.front;
}

bool operator!=(const vks::ColorBlendState& lhs, const vks::ColorBlendState& rhs)
{
	return std::tie(lhs.logic_op, lhs.logic_op_enable) != std::tie(rhs.logic_op, rhs.logic_op_enable) ||
		lhs.attachments.size() != rhs.attachments.size() ||
		!std::equal(lhs.attachments.begin(), lhs.attachments.end(), rhs.attachments.begin(),
			[](const vks::ColorBlendAttachmentState& lhs, const vks::ColorBlendAttachmentState& rhs) {
				return lhs == rhs;
			});
}

namespace vks
{
	void VulkanPipelineState::reset()
	{
		vertex_input_state = {};

		input_assembly_state = {};

		rasterization_state = {};

		multisample_state = {};

		depth_stencil_state = {};

		color_blend_state = {};
	}

	void VulkanPipelineState::set_vertex_input_state(const VertexInputState& new_vertex_input_state)
	{
		if (vertex_input_state != new_vertex_input_state)
		{
			vertex_input_state = new_vertex_input_state;
		}
	}

	void VulkanPipelineState::set_input_assembly_state(const InputAssemblyState& new_input_assembly_state)
	{
		if (input_assembly_state != new_input_assembly_state)
		{
			input_assembly_state = new_input_assembly_state;

		}
	}

	void VulkanPipelineState::set_rasterization_state(const RasterizationState& new_rasterization_state)
	{
		if (rasterization_state != new_rasterization_state)
		{
			rasterization_state = new_rasterization_state;
		}
	}

	void VulkanPipelineState::set_viewport_state(const ViewportState& new_viewport_state)
	{
		if (viewport_state != new_viewport_state)
		{
			viewport_state = new_viewport_state;

		}
	}

	void VulkanPipelineState::set_multisample_state(const MultisampleState& new_multisample_state)
	{
		if (multisample_state != new_multisample_state)
		{
			multisample_state = new_multisample_state;

		}
	}

	void VulkanPipelineState::set_depth_stencil_state(const DepthStencilState& new_depth_stencil_state)
	{
		if (depth_stencil_state != new_depth_stencil_state)
		{
			depth_stencil_state = new_depth_stencil_state;

		}
	}

	void VulkanPipelineState::set_color_blend_state(const ColorBlendState& new_color_blend_state)
	{
		if (color_blend_state != new_color_blend_state)
		{
			color_blend_state = new_color_blend_state;
		}
	}

	void VulkanPipelineState::set_pipeline_layout(VkPipelineLayout new_pipeline_layout)
	{
		pipelineLayout = new_pipeline_layout;
	}


	void VulkanPipelineState::set_render_pass(VkRenderPass new_render_pass)
	{
		render_pass = new_render_pass;
	}

	const VertexInputState& VulkanPipelineState::get_vertex_input_state() const
	{
		return vertex_input_state;
	}

	const InputAssemblyState& VulkanPipelineState::get_input_assembly_state() const
	{
		return input_assembly_state;
	}

	const RasterizationState& VulkanPipelineState::get_rasterization_state() const
	{
		return rasterization_state;
	}

	const ViewportState& VulkanPipelineState::get_viewport_state() const
	{
		return viewport_state;
	}

	const MultisampleState& VulkanPipelineState::get_multisample_state() const
	{
		return multisample_state;
	}

	const DepthStencilState& VulkanPipelineState::get_depth_stencil_state() const
	{
		return depth_stencil_state;
	}

	const ColorBlendState& VulkanPipelineState::get_color_blend_state() const
	{
		return color_blend_state;
	}

	VkPipelineLayout VulkanPipelineState::get_pipeline_layout() const
	{
		return pipelineLayout;
	}

	VkRenderPass VulkanPipelineState::get_render_pass() const
	{
		return render_pass;
	}

}


