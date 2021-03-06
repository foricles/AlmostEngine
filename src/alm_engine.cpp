#include "alm_engine.hpp"
#include "almCore/alm_log.hpp"
#include "almCore/alm_platform.hpp"
#include "almCore/almSceneSystem/alm_scenemanager.hpp"
#include "almCore/almEntitySystem/interface/alm_imgr.hpp"
#include "almCore/almRender/interface/alm_irendersys.hpp"
#include "almCore/almUtils/alm_timesystem.hpp"

#include <thread>

using namespace alme;

AlmostEngine::AlmostEngine()
	: m_quit(false)
	, m_mainWindow(nullptr)
	, m_timeSystem(nullptr)
	, m_sceneManager(nullptr)
	, m_renderSystem(nullptr)
	, m_entityManager(nullptr)
	, m_mainWindowInitializer(nullptr)
	, m_sceneManagerInitializer(nullptr)
	, m_renderSystemInitializer(nullptr)
	, m_entityManagerInitializer(nullptr)
{
}

AlmostEngine::~AlmostEngine()
{
	if (m_entityManager) delete m_entityManager;
	if (m_sceneManager) delete m_sceneManager;
	if (m_renderSystem) delete m_renderSystem;
	if (m_timeSystem) delete m_timeSystem;
	if (m_mainWindow) delete m_mainWindow;
}

const IAlmWindow & AlmostEngine::GetMainWindow() const
{
	return *m_mainWindow;
}

const AlmTimeSystem & AlmostEngine::GetTimeSystem() const
{
	return *m_timeSystem;
}

const AlmSceneManager & AlmostEngine::GetSceneManager() const
{
	return *m_sceneManager;
}

const IAlmRenderSystem & AlmostEngine::GetRenderSystem() const
{
	return *m_renderSystem;
}

const IAlmEntityManager & AlmostEngine::GetEntityManager() const
{
	return *m_entityManager;
}

void AlmostEngine::InititalizeSubsystems()
{
	m_mainWindow = m_mainWindowInitializer();
	m_timeSystem = new AlmTimeSystem(this);
	m_entityManager = m_entityManagerInitializer();
	m_sceneManager = m_sceneManagerInitializer();
	m_renderSystem = m_renderSystemInitializer();

	m_mainWindow->on_close.Add(this, [this]() mutable { m_quit = true; });
	m_mainWindow->canvas_size_callback.Add<IAlmRenderSystem>(m_renderSystem, &IAlmRenderSystem::OnWindowResize);

	m_renderSystem->InitRenderAPIInstance();
}

void AlmostEngine::RunLoop()
{
	m_sceneManager->RunScene(0);
	while (!m_quit)
	{
		m_timeSystem->BeginMeasurement();

		std::this_thread::sleep_for(std::chrono::milliseconds(16));

		m_mainWindow->Update();

		m_renderSystem->BeginRender();

		m_sceneManager->OnUpdate();

		m_renderSystem->FinishRender();

		m_sceneManager->PostUpdate();

		m_timeSystem->EndMeasurement();
	}
}
