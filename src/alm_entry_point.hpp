#ifndef _ALM_ENGINE_ENTRY_POINT_HPP_
#define _ALM_ENGINE_ENTRY_POINT_HPP_

#include "alm_engine.hpp"

namespace alme
{

class AlmApplicationDescriptor
{
public:
	uint32_t m_applicationVersion;
	std::string m_applicationName;
	std::vector<std::string> m_credits;

};

class AlmEngineEntryPoint
{
public:
	AlmEngineEntryPoint();
	virtual ~AlmEngineEntryPoint();

	virtual void InitializeEngineComponents();

	virtual void InitializeSsettings() = 0;
	virtual void RegisterGameScenes(AlmSceneManager *namager) = 0;

	virtual void Run() final;

	virtual void SetupApplicationInfo();
	uint32_t makeVersion(uint32_t major, uint32_t minor, uint32_t patch);

private:
	AlmApplicationDescriptor m_descriptor;
	AlmostEngine m_engine;

};
}
#endif // !_ALM_ENGINE_HPP_