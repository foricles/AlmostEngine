#include "alm_entry_point.hpp"
#include "almCore/almSceneSystem/alm_scenemanager.hpp"
#include "almCore/almEntitySystem/alm_entitymgr.hpp"
#include "almCore/almRender/openglrender/alm_glrender.hpp"
#include "almCore/almRender/vulkanrender/alm_vulkanrender.hpp"
#include "almCore/almUser/almWindow/alm_pcwindow.hpp"

using namespace alme;

AlmEngineEntryPoint::AlmEngineEntryPoint()
{
}

AlmEngineEntryPoint::~AlmEngineEntryPoint()
{
}

void AlmEngineEntryPoint::SetupApplicationInfo()
{
	m_descriptor.m_applicationName = "AlmostEngine";
	m_descriptor.m_applicationVersion = makeVersion(0, 1, 0);
	m_descriptor.m_credits.push_back("Eugeniy Kudriashov");
}

uint32_t alme::AlmEngineEntryPoint::makeVersion(uint32_t major, uint32_t minor, uint32_t patch)
{
	return (((major) << 22) | ((minor) << 12) | (patch));
}

void AlmEngineEntryPoint::InitializeEngineComponents()
{
	m_engine.SetSceneManager<AlmSceneManager>();
	m_engine.SetEntityManager<AlmEntityManager>();
	m_engine.SetRenderSystem<AlmGLRender>();
	m_engine.SetMainWindow<AlmWindowPC>();
}

void alme::AlmEngineEntryPoint::Run()
{
	SetupApplicationInfo();
	InitializeEngineComponents();
	InitializeSsettings();
	m_engine.InititalizeSubsystems();
	RegisterGameScenes(m_engine.m_sceneManager);
	m_engine.RunLoop();
}
