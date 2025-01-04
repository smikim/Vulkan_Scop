#include "BasicPSO.h"
#include "VulkanModel.h"

namespace Graphics
{
	BasicPSO::BasicPSO(vks::VulkanDevice& device, VkRenderPass renderPass)
		: _vulkanDevice(device), _RenderPass(renderPass)
	{
	}

	BasicPSO::~BasicPSO()
	{
		vkDestroyShaderModule(_vulkanDevice.getLogicalDevice(), _VertShaderModule, nullptr);
		vkDestroyShaderModule(_vulkanDevice.getLogicalDevice(), _FragShaderModule, nullptr);
	}

	void BasicPSO::initPSO()
	{
		vks::InputAssemblyState IAState;

		_pipelineState.set_input_assembly_state(IAState);

		vks::RasterizationState RState;

		RState.cull_mode = VK_CULL_MODE_NONE;
		RState.front_face = VK_FRONT_FACE_COUNTER_CLOCKWISE;

		_pipelineState.set_rasterization_state(RState);

		vks::ColorBlendState CBState;
		vks::ColorBlendAttachmentState blendAttachmentState;
		CBState.attachments.push_back(blendAttachmentState);
		_pipelineState.set_color_blend_state(CBState);

		vks::DepthStencilState DepthState;
		DepthState.depth_compare_op = VK_COMPARE_OP_LESS_OR_EQUAL;

		_pipelineState.set_depth_stencil_state(DepthState);

		vks::ViewportState ViewState;
		_pipelineState.set_viewport_state(ViewState);

		vks::MultisampleState MultiSampleState;
		_pipelineState.set_multisample_state(MultiSampleState);

		vks::VertexInputState VIState;
		VIState.bindings = vks::VulkanModel::Vertex::getBindingDescription();
		
		
		VIState.attributes = vks::VulkanModel::Vertex::getAttributeDescription();

		_pipelineState.set_vertex_input_state(VIState);

		_pipelineState.set_pipeline_layout(_PipelineLayout);
		_pipelineState.set_render_pass(_RenderPass);

		// Offscreen pipeline (vertex shader only)
		_VertShaderModule = utils::loadSPIRVShader("shaders/vert.spv", _vulkanDevice);
		_FragShaderModule = utils::loadSPIRVShader("shaders/frag.spv", _vulkanDevice);


	}

	void BasicPSO::setPipelineLayout(VkPipelineLayout pipelineLayout)
	{
		if (pipelineLayout != nullptr)
			_PipelineLayout = pipelineLayout;
	}
}