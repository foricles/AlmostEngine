#include "alm_engine.hpp"
#include "almCore/alm_platform.hpp"
#include "almCore/almEntitySystem/alm_imgr.hpp"
#include "almCore/almSceneSystem/alm_scenemanager.hpp"
#include "almCore/almRender/interface/alm_irendersys.hpp"

#include <thread>

using namespace alme;

AlmostEngine::AlmostEngine()
	: m_entityManager(nullptr)
	, m_renderSystem(nullptr)
	, m_sceneManager(nullptr)
{
}

AlmostEngine::~AlmostEngine()
{
	if (m_sceneManager) delete m_sceneManager;
	if (m_entityManager) delete m_entityManager;
	if (m_renderSystem) delete m_renderSystem;
}

void AlmostEngine::InititalizeSubsystems()
{
	m_entityManager = m_entityManagerInitializer();
	m_sceneManager = m_sceneManagerInitializer();
	m_renderSystem = m_renderSystemInitializer();


	m_renderSystem->InitRenderAPIInstance();
}

void AlmostEngine::RunLoop()
{
	m_sceneManager->RunScene(0);
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}
