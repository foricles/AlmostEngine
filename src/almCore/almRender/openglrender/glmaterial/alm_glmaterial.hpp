#ifndef _ALM_RENDER_MATERIAL_OPENGL_HPP_
#define _ALM_RENDER_MATERIAL_OPENGL_HPP_

#include "../src/almCore/almRender/interface/alm_imaterial.hpp"
#include <set>
#include <utility>

namespace alme
{

struct sAlmUniformCache
{
	int32_t location;
	std::string name;

	sAlmUniformCache(const std::string &ident) : location(-1), name(ident) {}
	bool operator < (const sAlmUniformCache &rhv) const { return name < rhv.name; };
};




class AlmGLMaterial : public IAlmRenderMaterial
{
public:
	AlmGLMaterial() = delete;
	AlmGLMaterial(const AlmGLMaterial &rhv) = delete;
	AlmGLMaterial & operator=(const AlmGLMaterial &rhv) = delete;

public:
	~AlmGLMaterial();

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

	void SetShader(const std::string &shaderpath) override;
	void SetShader(const std::string &shaderpath, eShaderType type) override;

	void Bind() override;
	void Unbind() override;

private:
	int32_t GetUniformLocation(const std::string &name);
	std::string LoadShader(const std::string &path);
	void CompileShader(const std::string &body, eShaderType type);

private:
	std::set<sAlmUniformCache>			m_uniforms;
	kmu::vec4							m_scissorBox;
	ePoligonCullMode					m_cullMode;
	ePoligonDrawMode					m_poligonMode;
};

}
#endif // !_ALM_RENDER_MATERIAL_OPENGL_HPP_