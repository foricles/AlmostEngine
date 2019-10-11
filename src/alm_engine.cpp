#include "alm_engine.hpp"
#include "almCore/alm_platform.hpp"
#include "almCore/almEntitySystem/alm_imgr.hpp"
#include "almCore/almSceneSystem/alm_scenemanager.hpp"

using namespace alme;

AlmostEngine::AlmostEngine()
	: m_entityManager(nullptr)
	, m_sceneManager(nullptr)
{
}

AlmostEngine::~AlmostEngine()
{
	if (m_sceneManager) delete m_sceneManager;
	if (m_entityManager) delete m_entityManager;
}