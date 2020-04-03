#include "alm_scenemanager.hpp"
#include "../src/alm_engine.hpp"
#include "../almSceneSystem/alm_scene.hpp"
#include "../src/almCore/almEntitySystem/interface/alm_imgr.hpp"

#include <algorithm>

alme::AlmSceneManager::AlmSceneManager(AlmostEngine *engine)
	: AlmEngineSystem(engine)
	, m_activeScene(nullptr)
{
}

alme::AlmSceneManager::~AlmSceneManager()
{
	if (m_activeScene)
	{
		m_activeScene->OnDelete();
		delete m_activeScene;
	}
}

void alme::AlmSceneManager::RunScene(const std::string & name)
{
	auto find = std::find_if(m_scenes.begin(), m_scenes.end(), [name](const CreateCallback &cll) {
		return cll.first == name;
	});

	if (find == m_scenes.end())
		return;

	RunGameScene(find->second());
}

void alme::AlmSceneManager::RunScene(uint32_t id)
{
	RunGameScene(m_scenes[id].second());
}

void alme::AlmSceneManager::RunGameScene(AlmGameScene * scene)
{
	auto manager = const_cast<IAlmEntityManager*>(&Engine()->GetEntityManager());
	manager->ReleaseAllEntities();

	if (m_activeScene)
	{
		m_activeScene->OnDelete();
		delete m_activeScene;
	}

	m_activeScene = scene;
	m_activeScene->SetEnginePtr(Engine());
	m_activeScene->OnStart();
}

void alme::AlmSceneManager::OnUpdate()
{
	if (m_activeScene)
		m_activeScene->OnUpdate();
}

void alme::AlmSceneManager::PostUpdate()
{
	if (m_activeScene)
		m_activeScene->PostUpdate();
}