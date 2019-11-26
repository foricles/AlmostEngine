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

		vk::Buffer indexBuffer;
		vk::DeviceMemory indexBufferMemory;
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

	std::string body(shader.Load());

	vk::ShaderModuleCreateInfo shaderInfo;
	shaderInfo.setCodeSize(body.size());
	shaderInfo.setPCode(reinterpret_cast<const uint32_t*>(body.data()));

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
}

void AlmVkMaterial::Unbind()
{
}