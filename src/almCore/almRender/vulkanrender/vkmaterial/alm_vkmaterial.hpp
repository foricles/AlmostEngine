#ifndef _ALM_RENDER_MATERIAL_HPP_
#define _ALM_RENDER_MATERIAL_HPP_

#include "../src/almCore/almRender/interface/alm_imaterial.hpp"
#include <vector>

namespace alme
{
struct sAlmVulkanContext;
class AlmVkMaterial : public IAlmRenderMaterial
{
public:
	AlmVkMaterial(sAlmVulkanContext *context);

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

private:
	sAlmVulkanContext *m_context;
	kmu::vec4 m_scissorBox;
	ePoligonCullMode m_cullMode;
	ePoligonDrawMode m_poligonMode;
};

}
#endif // !_ALM_RENDER_MATERIAL_HPP_