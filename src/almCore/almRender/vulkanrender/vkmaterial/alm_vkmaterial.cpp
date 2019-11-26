#include "alm_vkmaterial.hpp"
#include "../src/almCore/almFileSystem/alm_file.hpp"
#include "../src/almCore/alm_log.hpp"
#include "../src/almCore/almRender/vulkanrender/alm_vkinstance.hpp"

namespace alme
{
	struct sVkMatVaribles 
	{
		std::vector<eShaderType> shaderTypes;
		std::vector<vk::PipelineShaderStageCreateInfo> shaderStage;
		vk::PipelineShaderStageCreateInfo getStage(eShaderType type)
		{
			size_t i;
			for (i = 0; i < shaderTypes.size(); ++i)
				if (shaderTypes[i] == type) break;
			return shaderStage[i];
		}

		vk::Pipeline graphicsPipeline;
		vk::PipelineLayout pipelineLayout;
	};
}

using namespace alme;

AlmVkMaterial::AlmVkMaterial(sAlmVulkanContext *context)
	: IAlmRenderMaterial()
	, m_var(new sVkMatVaribles())
	, m_context(context)
	, m_scissorBox(0, 0, 1, 1)
	, m_cullMode(ePoligonCullMode::eBack)
	, m_poligonMode (ePoligonDrawMode::eFill)
{
}

AlmVkMaterial::~AlmVkMaterial()
{

}

uint32_t AlmVkMaterial::GetId() const
{
	return 0;
}

std::string AlmVkMaterial::GetName() const
{
	return "Material";
}

void AlmVkMaterial::SetUniform(const std::string & name, int value)
{
}

void AlmVkMaterial::SetUniform(const std::string & name, float value)
{
}

void AlmVkMaterial::SetUniform(const std::string & name, const kmu::vec2 & value)
{
}

void AlmVkMaterial::SetUniform(const std::string & name, const kmu::vec3 & value)
{
}

void AlmVkMaterial::SetUniform(const std::string & name, const kmu::vec4 & value)
{
}

void AlmVkMaterial::SetUniform(const std::string & name, const kmu::mat4 & value)
{
}

kmu::vec4 AlmVkMaterial::GetScissorBox() const
{
	return m_scissorBox;
}

void AlmVkMaterial::SetScreenScissorBox(const kmu::vec4 & box)
{
	m_scissorBox = box;
}

void AlmVkMaterial::SetScreenScissorBox(const kmu::vec2 & leftTop, const kmu::vec2 & rightBottom)
{
	m_scissorBox.x = leftTop.x;
	m_scissorBox.y = leftTop.y;
	m_scissorBox.z = rightBottom.x;
	m_scissorBox.w = rightBottom.y;
}

ePoligonDrawMode AlmVkMaterial::GetPoligonDrawMode() const
{
	return m_poligonMode;
}

void AlmVkMaterial::SetPoligonDrawMode(ePoligonDrawMode mode)
{
	m_poligonMode = mode;
}

ePoligonCullMode alme::AlmVkMaterial::GetPoligonCullMode() const
{
	return m_cullMode;
}

void alme::AlmVkMaterial::GetPoligonCullMode(ePoligonCullMode mode)
{
	m_cullMode = mode;
}

void AlmVkMaterial::SetShader(const std::string & shaderpath, eShaderType type)
{
	for (eShaderType stype : m_var->shaderTypes)
	{
		if (stype == type)
		{
			ALM_LOG_ERROR("Shader with type", (int)type, "alredy exist");
			return;
		}
	}

	io::AlmFile shader(shaderpath);
	if (!shader.Exist())
	{
		ALM_LOG_ERROR("Shader not exist", shaderpath);
		return;
	}

	shader.Load();

	vk::ShaderModuleCreateInfo shaderInfo;
	shaderInfo.setCodeSize(shader.asBin().size());
	shaderInfo.setPCode(reinterpret_cast<const uint32_t*>(shader.asBin().data()));

	auto shaderModule = m_context->device.createShaderModule(shaderInfo);
	if (shaderModule.result != vk::Result::eSuccess)
	{
		ALM_LOG_ERROR("Failed to create shader", shaderpath);
		return;
	}

	vk::PipelineShaderStageCreateInfo shaderStageInfo;
	shaderStageInfo.setModule(shaderModule.value);
	shaderStageInfo.setPName("main");

	switch (type)
	{
	case eShaderType::eVertex: shaderStageInfo.setStage(vk::ShaderStageFlagBits::eVertex); break;
	case eShaderType::eFragment: shaderStageInfo.setStage(vk::ShaderStageFlagBits::eFragment); break;
	case eShaderType::eGeometry: shaderStageInfo.setStage(vk::ShaderStageFlagBits::eGeometry); break;
	}

	m_var->shaderTypes.push_back(type);
	m_var->shaderStage.push_back(shaderStageInfo);

	m_context->device.destroyShaderModule(shaderModule.value);
}

void AlmVkMaterial::Bind()
{
	vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
	vertexInputInfo.setVertexBindingDescriptionCount(0);
	vertexInputInfo.setPVertexBindingDescriptions(nullptr);
	vertexInputInfo.setVertexAttributeDescriptionCount(0);
	vertexInputInfo.setPVertexAttributeDescriptions(nullptr);

	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
	inputAssembly.setTopology(vk::PrimitiveTopology::eTriangleList);
	inputAssembly.setPrimitiveRestartEnable(false);

	vk::Viewport viewport;
	viewport.setX(0).setY(0);
	viewport.setMinDepth(0).setMaxDepth(1);
	viewport.setWidth(static_cast<float>(m_context->swapChainExtent.width));
	viewport.setHeight(static_cast<float>(m_context->swapChainExtent.height));

	vk::Rect2D scissor = {};
	scissor.setOffset(vk::Offset2D(0, 0));
	scissor.setExtent(m_context->swapChainExtent);

	vk::PipelineViewportStateCreateInfo viewportState;
	viewportState.setScissorCount(1);
	viewportState.setViewportCount(1);
	viewportState.setPScissors(&scissor);
	viewportState.setPViewports(&viewport);

	vk::PipelineRasterizationStateCreateInfo rasterizer;
	rasterizer.setDepthClampEnable(false);
	rasterizer.setRasterizerDiscardEnable(false);
	rasterizer.setPolygonMode(vk::PolygonMode::eFill);
	rasterizer.setLineWidth(1);
	rasterizer.setCullMode(vk::CullModeFlagBits::eBack);
	rasterizer.setFrontFace(vk::FrontFace::eCounterClockwise);
	rasterizer.setDepthBiasEnable(false);
	rasterizer.setDepthBiasConstantFactor(0);
	rasterizer.setDepthBiasClamp(0);
	rasterizer.setDepthBiasSlopeFactor(0);

	vk::PipelineMultisampleStateCreateInfo multisampling;
	multisampling.setSampleShadingEnable(false);
	multisampling.setRasterizationSamples(vk::SampleCountFlagBits::e2);
	multisampling.setMinSampleShading(1);
	multisampling.setPSampleMask(nullptr);
	multisampling.setAlphaToCoverageEnable(false);
	multisampling.setAlphaToOneEnable(false);

	vk::PipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
	colorBlendAttachment.setBlendEnable(false);
	colorBlendAttachment.setSrcColorBlendFactor(vk::BlendFactor::eOne);
	colorBlendAttachment.setDstColorBlendFactor(vk::BlendFactor::eZero);
	colorBlendAttachment.setColorBlendOp(vk::BlendOp::eAdd);
	colorBlendAttachment.setSrcAlphaBlendFactor(vk::BlendFactor::eOne);
	colorBlendAttachment.setDstAlphaBlendFactor(vk::BlendFactor::eZero);
	colorBlendAttachment.setAlphaBlendOp(vk::BlendOp::eAdd);

	vk::PipelineColorBlendStateCreateInfo colorBlending;
	colorBlending.setLogicOpEnable(false);
	colorBlending.setLogicOp(vk::LogicOp::eCopy);
	colorBlending.setAttachmentCount(1);
	colorBlending.setPAttachments(&colorBlendAttachment);
	colorBlending.setBlendConstants({ {0, 0, 0, 0} });

	vk::DynamicState dynamicStates[] = { vk::DynamicState::eViewport, vk::DynamicState::eLineWidth };

	vk::PipelineDynamicStateCreateInfo dynamicState;
	dynamicState.setDynamicStateCount(2);
	dynamicState.setPDynamicStates(dynamicStates);

	vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.setSetLayoutCount(0);
	pipelineLayoutInfo.setPushConstantRangeCount(0);

	m_var->pipelineLayout = m_context->device.createPipelineLayout(pipelineLayoutInfo).value;

	vk::GraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.setStageCount(m_var->shaderStage.size());
	pipelineInfo.setPStages(m_var->shaderStage.data());
	pipelineInfo.setPVertexInputState(&vertexInputInfo);
	pipelineInfo.setPInputAssemblyState(&inputAssembly);
	pipelineInfo.setPViewportState(&viewportState);
	pipelineInfo.setPRasterizationState(&rasterizer);
	pipelineInfo.setPMultisampleState(&multisampling);
	pipelineInfo.setPColorBlendState(&colorBlending);
	pipelineInfo.setLayout(m_var->pipelineLayout);
	pipelineInfo.setRenderPass(m_context->renderPass);
	pipelineInfo.setSubpass(0);
	pipelineInfo.setBasePipelineIndex(-1);

	m_var->graphicsPipeline = m_context->device.createGraphicsPipeline(vk::PipelineCache(), pipelineInfo).value;
}

void AlmVkMaterial::Unbind()
{
	m_context->device.destroyPipeline(m_var->graphicsPipeline);
	m_context->device.destroyPipelineLayout(m_var->pipelineLayout);
}