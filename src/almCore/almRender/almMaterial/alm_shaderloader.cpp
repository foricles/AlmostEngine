#include "alm_shaderloader.hpp"

using namespace alme;

AlmShaderLoader::AlmShaderLoader()
{
}

AlmShaderLoader::~AlmShaderLoader()
{
}

std::string AlmShaderLoader::LoadShader(const std::string filepath, eShaderType type, const AlmShaderLoader & loader)
{
	return loader.loadShader(filepath, type);
}

std::string AlmShaderLoader::loadShader(const std::string filepath, eShaderType type) const
{
	return std::string();
}
