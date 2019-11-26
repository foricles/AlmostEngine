#ifndef _ALM_RENDER_MATERIAL_HPP_
#define _ALM_RENDER_MATERIAL_HPP_

#include "../src/almCore/almRender/interface/alm_imaterial.hpp"
#include <vector>
#include <memory>

namespace alme
{
struct sVkMatVaribles;
struct sAlmVulkanContext;
class AlmVkMaterial : public IAlmRenderMaterial
{
public:
	AlmVkMaterial() = delete;
	AlmVkMaterial(const AlmVkMaterial &rhv) = delete;
	AlmVkMaterial & operator=(const AlmVkMaterial &rhv) = delete;

public:
	AlmVkMaterial(sAlmVulkanContext *context);
	~AlmVkMaterial();

	uint32_t GetId() const override;
	std::string GetName() const override;

	void SetUniform(const std::string &name, int value) override;
	void SetUniform(const std::string &name, float value) override;

	void SetUniform(const std::string &name, const kmu::vec2 &value) override;
	void SetUniform(const std::string &name, const kmu::vec3 &value) override;
	void SetUniform(const std::string &name, const kmu::vec4 &value) override;

	void SetUniform(const std::string &name, const kmu::mat4 &value) override;

	kmu::vec4 GetScissorBox() const override;
	void SetScreenScissorBox(const kmu::vec4 &box) override;
	void SetScreenScissorBox(const kmu::vec2 &leftTop, const kmu::vec2 &rightBottom) override;

	ePoligonDrawMode GetPoligonDrawMode() const override;
	void SetPoligonDrawMode(ePoligonDrawMode mode) override;

	ePoligonCullMode GetPoligonCullMode() const override;
	void GetPoligonCullMode(ePoligonCullMode mode) override;

	void SetShader(const std::string &shaderpath, eShaderType type) override;

	void Bind() override;
	void Unbind() override;

private:
	std::unique_ptr<sVkMatVaribles>		m_var;
	sAlmVulkanContext				   *m_context;
	kmu::vec4							m_scissorBox;
	ePoligonCullMode					m_cullMode;
	ePoligonDrawMode					m_poligonMode;
};

}
#endif // !_ALM_RENDER_MATERIAL_HPP_