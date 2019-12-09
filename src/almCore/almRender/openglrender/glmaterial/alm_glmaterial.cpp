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

	std::vector<std::pair<std::string, eShaderType>> parsed;
	ParseShaders(parsed, body);

	std::vector<uint32_t> compiled;
	compiled.reserve(parsed.size());
	CompileShader(compiled, parsed);

	LinkShadersIntoProgram(compiled);
}

void AlmGLMaterial::SetShader(const std::string & shaderpath, eShaderType type)
{
	std::string body = LoadShader(shaderpath);
	if (body.size() <= 0) return;

	std::vector<std::pair<std::string, eShaderType>> parsed;
	parsed.emplace_back(body, type);
	std::vector<uint32_t> compiled;
	CompileShader(compiled, parsed);
	LinkShadersIntoProgram(compiled);
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

	glUseProgram(m_ProgramId);
}

void AlmGLMaterial::Unbind()
{
	glUseProgram(0);
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

void AlmGLMaterial::ParseShaders(std::vector<std::pair<std::string, eShaderType>>& parsed, const std::string & body)
{
	if (m_ProgramId != 0)
	{
		ALM_LOG_WARNING("Shader for material will be replaced");
		glDeleteProgram(m_ProgramId);
		m_uniforms.clear();
	}

	m_ProgramId = glCreateProgram();

	const char * kVertexToken = "#_alm_vertex_#";
	const char * kGeometryToken = "#_alm_geometry_#";
	const char * kFragmentToken = "#_alm_fragment_#";

	struct {
		uint32_t pos;
		eShaderType type;
		uint32_t offset;
	} tokens [3] = {
		{body.find(kVertexToken), eShaderType::eVertex, std::strlen(kVertexToken)},
		{body.find(kGeometryToken), eShaderType::eGeometry, std::strlen(kGeometryToken)},
		{body.find(kFragmentToken), eShaderType::eFragment, std::strlen(kFragmentToken)}
	};

	if (tokens[0].pos == tokens[1].pos == tokens[2].pos == std::string::npos)
	{
		ALM_LOG_ERROR("Could not find any shader!");
		glDeleteProgram(m_ProgramId);
		m_ProgramId = 0;
		return;
	}

	if (tokens[0].pos > tokens[1].pos)
		std::swap(tokens[0], tokens[1]);
	if (tokens[0].pos > tokens[2].pos)
		std::swap(tokens[0], tokens[2]);
	if (tokens[1].pos > tokens[2].pos)
		std::swap(tokens[1], tokens[2]);

	parsed.push_back(std::pair<std::string, eShaderType>(body.substr(tokens[0].pos + tokens[0].offset, tokens[1].pos - tokens[0].pos - tokens[0].offset), tokens[0].type));
	if (tokens[1].pos != std::string::npos)
		parsed.push_back(std::pair<std::string, eShaderType>(body.substr(tokens[1].pos + tokens[1].offset, tokens[2].pos - tokens[1].pos - tokens[1].offset), tokens[1].type));
	if (tokens[2].pos != std::string::npos)
		parsed.push_back(std::pair<std::string, eShaderType>(body.substr(tokens[2].pos + tokens[2].offset), tokens[2].type));
}

void AlmGLMaterial::CompileShader(std::vector<uint32_t>& compiled, const std::vector<std::pair<std::string, eShaderType>>& parsed)
{
	for (const auto &shader : parsed)
	{
		uint32_t shId = 0;
		switch (shader.second)
		{
		case eShaderType::eVertex:   shId = glCreateShader(GL_VERTEX_SHADER);   break;
		case eShaderType::eGeometry: shId = glCreateShader(GL_GEOMETRY_SHADER); break;
		case eShaderType::eFragment: shId = glCreateShader(GL_FRAGMENT_SHADER); break;
		}

		const GLchar *src[1];
		GLint lng[1];
		src[0] = shader.first.data();

		ALM_LOG_ASSERT(src[0] != nullptr, "Failed to create vertex shader");

		lng[0] = strlen(src[0]);
		glShaderSource(shId, 1, src, lng);

		glCompileShader(shId);

		ALM_LOG_ASSERT(shId != 0, "Failed to create vertex shader");

		//check to compile success
		GLint success = 0;
		glGetShaderiv(shId, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint length = 0;
			glGetShaderiv(shId, GL_INFO_LOG_LENGTH, &length);

			std::vector<char> errors(length);
			glGetShaderInfoLog(shId, length, &length, &errors[0]);

			glDeleteShader(shId);

			ALM_LOG_ERROR(&errors[0]);
			ALM_LOG_ERROR("Shader compile failed!");
		}

		glAttachShader(m_ProgramId, shId);
		compiled.push_back(shId);
	}
}

void AlmGLMaterial::LinkShadersIntoProgram(std::vector<uint32_t>& compiled)
{
	glLinkProgram(m_ProgramId);
	//success checked
	GLint isLinked = 0;
	glGetProgramiv(m_ProgramId, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_ProgramId, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> infoLog(maxLength);
		glGetProgramInfoLog(m_ProgramId, maxLength, &maxLength, &infoLog[0]);

		for (GLuint var : compiled)
			glDeleteShader(var);
		glDeleteProgram(m_ProgramId);
		m_ProgramId = 0;

		ALM_LOG_ERROR(&infoLog[0]);
		ALM_LOG_ERROR("Shader link failed");
	}

	glValidateProgram(m_ProgramId);

	//Always detach shaders after a successful link.
	for (GLuint var : compiled)
	{
		if (var > 0)
		{
			glDetachShader(m_ProgramId, var);
			glDeleteShader(var);
		}
	}
}
