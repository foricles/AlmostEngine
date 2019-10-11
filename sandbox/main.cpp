#include <alm_almost_engine.hpp>
#include <almCore/almSceneSystem/alm_scenemanager.hpp>
#include "simple_scene.hpp"

class SanboxApplication : public alme::AlmEngineEntryPoint
{
public:
	void InitializeSsettings() override
	{

	}

	void RegisterGameScenes(alme::AlmSceneManager *namager) override
	{
		namager->AddScene<SimpleScene>("simple_scene");
	}
};

alme::AlmEngineEntryPoint *CreateEntryPoint()
{
	return new SanboxApplication();
}