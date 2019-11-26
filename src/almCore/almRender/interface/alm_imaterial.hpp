#ifndef _ALM_RENDER_MATERIAL_INTERFACE_HPP_
#define _ALM_RENDER_MATERIAL_INTERFACE_HPP_

#include "../src/almCore/almMath/matrices.hpp"

namespace alme
{

	enum class eShaderType
	{
		eVertex,
		eFragment,
		eGeometry
	};

	enum class ePoligonDrawMode
	{
		eFill,
		eLine,
		ePoint
	};

	enum class ePoligonCullMode
	{
		eBack,
		eFront
	};

class IAlmRenderMaterial
{

public:
	virtual ~IAlmRenderMaterial() {}

	virtual uint32_t			GetId() const = 0;
	virtual std::string			GetName() const = 0;

	virtual void				SetUniform(const std::string &name, int value) = 0;
	virtual void				SetUniform(const std::string &name, float value) = 0;

	virtual void				SetUniform(const std::string &name, const kmu::vec2 &value) = 0;
	virtual void				SetUniform(const std::string &name, const kmu::vec3 &value) = 0;
	virtual void				SetUniform(const std::string &name, const kmu::vec4 &value) = 0;

	virtual void				SetUniform(const std::string &name, const kmu::mat4 &value) = 0;

	virtual kmu::vec4			GetScissorBox() const = 0;
	virtual void				SetScreenScissorBox(const kmu::vec4 &box) = 0;
	virtual void				SetScreenScissorBox(const kmu::vec2 &leftTop, const kmu::vec2 &rightBottom) = 0;

	virtual ePoligonDrawMode	GetPoligonDrawMode() const = 0;
	virtual void				SetPoligonDrawMode(ePoligonDrawMode mode) = 0;

	virtual ePoligonCullMode	GetPoligonCullMode() const = 0;
	virtual void				GetPoligonCullMode(ePoligonCullMode mode) = 0;

	virtual void				SetShader(const std::string &shaderpath, eShaderType type) = 0;

	virtual void				Bind() = 0;
	virtual void				Unbind() = 0;
};

}


#endif // !_ALM_RENDER_MATERIAL_INTERFACE_HPP_