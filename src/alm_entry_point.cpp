#include "alm_entry_point.hpp"
#include "almCore/almSceneSystem/alm_scenemanager.hpp"
#include "almCore/almEntitySystem/alm_entitymgr.hpp"
#include "almCore/almRender/alm_vulkanrender.hpp"

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
	m_engine.SetRenderSystem<AlmVulkanRender>();
}

void alme::AlmEngineEntryPoint::Run()
{
	InitializeEngineComponents();
	InitializeSsettings();
	m_engine.InititalizeSubsystems();
	RegisterGameScenes(m_engine.m_sceneManager);
	m_engine.RunLoop();
}
