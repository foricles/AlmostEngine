#ifndef _ALM_SHADER_LOADER_HPP_
#define _ALM_SHADER_LOADER_HPP_

#include <string>

namespace alme
{
	enum class eShaderType
	{
		eVertex,
		eFragment,
		eGeometry
	};

	class AlmShaderLoader
	{
	public:
		AlmShaderLoader();
		virtual ~AlmShaderLoader();

		static std::string LoadShader(const std::string filepath, eShaderType type, const AlmShaderLoader &loader = AlmShaderLoader());

	protected:
		virtual std::string loadShader(const std::string filepath, eShaderType type) const;
	};
}
#endif // !_ALM_SHADER_LOADER_HPP_