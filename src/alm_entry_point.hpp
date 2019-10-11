#ifndef _ALM_ENGINE_ENTRY_POINT_HPP_
#define _ALM_ENGINE_ENTRY_POINT_HPP_

#include "alm_engine.hpp"

namespace alme
{
class AlmEngineEntryPoint
{
public:
	AlmEngineEntryPoint();
	virtual ~AlmEngineEntryPoint();

	virtual void InitializeEngineComponents();

	virtual void InitializeSsettings() = 0;
	virtual void RegisterGameScenes(AlmSceneManager *namager) = 0;

	virtual void Run() final;

private:
	AlmostEngine m_engine;

};
}
#endif // !_ALM_ENGINE_HPP_