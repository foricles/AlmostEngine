#include "alm_entry_point.hpp"
#include "almCore/almSceneSystem/alm_scenemanager.hpp"
#include "almCore/almEntitySystem/alm_entitymgr.hpp"

using namespace alme;

AlmEngineEntryPoint::AlmEngineEntryPoint()
{
}

AlmEngineEntryPoint::~AlmEngineEntryPoint()
{
}

void AlmEngineEntryPoint::InitializeEngineComponents()
{
	m_engine.SetSceneManager<AlmSceneManager>();
	m_engine.SetEntityManager<AlmEntityManager>();
}

void alme::AlmEngineEntryPoint::Run()
{
	InitializeEngineComponents();
	m_engine.m_sceneManager = m_engine.m_sceneManagerInitializer();

	InitializeSsettings();
	RegisterGameScenes(m_engine.m_sceneManager);


	m_engine.m_sceneManager->RunScene(0);
}
