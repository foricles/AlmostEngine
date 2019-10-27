#include "alm_scenemanager.hpp"
#include "../almSceneSystem/alm_iscene.hpp"

#include <algorithm>

alme::AlmSceneManager::AlmSceneManager()
	: m_activeScene(nullptr)
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

	if (m_activeScene)
	{
		m_activeScene->OnDelete();
		delete m_activeScene;
	}
	m_activeScene = find->second();
	m_activeScene->OnStart();
}

void alme::AlmSceneManager::RunScene(uint32_t id)
{
	if (m_activeScene)
	{
		m_activeScene->OnDelete();
		delete m_activeScene;
	}
	m_activeScene = m_scenes[id].second();
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