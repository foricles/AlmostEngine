#include "alm_glmaterial.hpp"
#include "../src/almCore/almFileSystem/alm_file.hpp"
#include "../src/almCore/alm_log.hpp"
#include <glew/glew.h>

using namespace alme;

AlmGLMaterial::~AlmGLMaterial()
{
}

uint32_t AlmGLMaterial::GetId() const
{
	return uint32_t();
}
std::string AlmGLMaterial::GetName() const
{
	return std::string();
}

void AlmGLMaterial::SetUniform(const std::string & name, int value)
{
	GLint uil = GetUniformLocation(name);
	if (uil >= 0) glUniform1i(uil, value);
}

void AlmGLMaterial::SetUniform(const std::string & name, float value)
{
	GLint uil = GetUniformLocation(name);
	if (uil >= 0) glUniform1f(uil, value);
}

void AlmGLMaterial::SetUniform(const std::string & name, const kmu::vec2 & value)
{
	GLint uil = GetUniformLocation(name);
	if (uil >= 0) glUniform2f(uil, value.x, value.y);
}

void AlmGLMaterial::SetUniform(const std::string & name, const kmu::vec3 & value)
{
	GLint uil = GetUniformLocation(name);
	if (uil >= 0) glUniform3f(uil, value.x, value.y, value.z);
}

void AlmGLMaterial::SetUniform(const std::string & name, const kmu::vec4 & value)
{
	GLint uil = GetUniformLocation(name);
	if (uil >= 0) glUniform4f(uil, value.x, value.y, value.z, value.w);
}

void AlmGLMaterial::SetUniform(const std::string & name, const kmu::mat4 & value)
{
	GLint uil = GetUniformLocation(name);
	if (uil >= 0) glUniformMatrix4fv(uil, 1, GL_FALSE, &value[0]);
}

kmu::vec4 AlmGLMaterial::GetScissorBox() const
{
	return m_scissorBox;
}

void AlmGLMaterial::SetScreenScissorBox(const kmu::vec4 & box)
{
	m_scissorBox = box;
}

void AlmGLMaterial::SetScreenScissorBox(const kmu::vec2 & leftTop, const kmu::vec2 & rightBottom)
{
	m_scissorBox.x = leftTop.x;			m_scissorBox.y = leftTop.y;
	m_scissorBox.z = rightBottom.x;		m_scissorBox.w = rightBottom.y;
}

ePoligonDrawMode AlmGLMaterial::GetPoligonDrawMode() const
{
	return m_poligonMode;
}

void AlmGLMaterial::SetPoligonDrawMode(ePoligonDrawMode mode)
{
	m_poligonMode = mode;
}

ePoligonCullMode AlmGLMaterial::GetPoligonCullMode() const
{
	return m_cullMode;
}

void AlmGLMaterial::GetPoligonCullMode(ePoligonCullMode mode)
{
	m_cullMode = mode;
}

void AlmGLMaterial::SetShader(const std::string & shaderpath)
{
	std::string body = LoadShader(shaderpath);
	if (body.size() <= 0) return;

	std::vector<std::pair<std::string, eShaderType>> shaderList;

	//TODO: parse

	for (auto &pair : shaderList)
		CompileShader(pair.first, pair.second);
}

void AlmGLMaterial::SetShader(const std::string & shaderpath, eShaderType type)
{
	std::string body = LoadShader(shaderpath);
	if (body.size() <= 0) return;
	CompileShader(body, type);
}

void AlmGLMaterial::Bind()
{
	GLenum cull, mode;
	switch (m_cullMode)
	{
	case ePoligonCullMode::eBack:	cull = GL_BACK; break;
	case ePoligonCullMode::eFront:	cull = GL_FRONT; break;
	}

	switch (m_poligonMode)
	{
	case ePoligonDrawMode::eFill:	mode = GL_FILL; break;
	case ePoligonDrawMode::eLine:	mode = GL_LINE; break;
	case ePoligonDrawMode::ePoint:	mode = GL_POINT; break;
	}
	glPolygonMode(cull, mode);


	GLboolean isScissors = glIsEnabled(GL_SCISSOR_TEST);
	bool isZero = std::fabs(m_scissorBox.sqrMagnitude()) <= std::numeric_limits<float>::epsilon();
	if (isZero && (isScissors == GL_TRUE))
	{
		glDisable(GL_SCISSOR_BOX);
		isScissors = GL_FALSE;
	}
	else if (!isZero && (isScissors == GL_FALSE))
	{
		glEnable(GL_SCISSOR_BOX);
		isScissors = GL_TRUE;
	}

	if (isScissors == GL_TRUE)
	{
		uint32_t w = static_cast<float>(m_scissorBox.z - m_scissorBox.x);
		uint32_t h = static_cast<float>(m_scissorBox.w - m_scissorBox.y);
		glScissor(m_scissorBox.x, m_scissorBox.y, w, h);
	}
}

void AlmGLMaterial::Unbind()
{
}

int32_t AlmGLMaterial::GetUniformLocation(const std::string & name)
{
	sAlmUniformCache cache(name);

	auto fnd = m_uniforms.find(cache);
	if (fnd != m_uniforms.end())
		return fnd->location;

	cache.location = glGetUniformLocation(-1, name.c_str());
	if (cache.location != 0)
	{
		m_uniforms.insert(std::move(cache));
		return cache.location;
	}

	ALM_LOG_ERROR("Can not find uniform variable:", name);
	return -1;
}

std::string AlmGLMaterial::LoadShader(const std::string & path)
{
	io::AlmFile shader(path);
	if (!shader.Exist())
	{
		ALM_LOG_ERROR("Shader not exist:", path);
		return "";
	}
	shader.Load();
	return std::move(shader.asString());
}

void AlmGLMaterial::CompileShader(const std::string & body, eShaderType type)
{
}