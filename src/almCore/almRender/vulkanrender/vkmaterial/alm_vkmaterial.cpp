#include "alm_vkmaterial.hpp"
#include "../src/almCore/almFileSystem/alm_file.hpp"
#include "../src/almCore/alm_log.hpp"
#include "../src/almCore/almRender/vulkanrender/alm_vkinstance.hpp"

using namespace alme;

AlmVkMaterial::AlmVkMaterial(sAlmVulkanContext *context)
	: IAlmRenderMaterial()
	, m_context(context)
	, m_scissorBox(0, 0, 1, 1)
	, m_cullMode(ePoligonCullMode::eBack)
	, m_poligonMode (ePoligonDrawMode::eFill)
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
