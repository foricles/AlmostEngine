#include "alm_scene.hpp"
#include "../src/alm_engine.hpp"
#include "../src/almCore/almUser/almWindow/alm_iwindow.hpp"
#include "../src/almCore/almEntitySystem/interface/alm_imgr.hpp"
#include "../src/almCore/almRender/interface/alm_irendersys.hpp"
#include "../src/almCore/almEntitySystem/interface/alm_ientity.hpp"
#include "../src/almCore/almEntitySystem/interface/alm_itransform.hpp"



using namespace alme;

AlmGameScene::AlmGameScene()
	: m_engine(nullptr)
{

}

AlmGameScene::~AlmGameScene()
{

}

void AlmGameScene::SetEnginePtr(AlmostEngine * engine)
{
	m_engine = engine;
}

const AlmostEngine * AlmGameScene::Engine() const
{
	return m_engine;
}

IAlmEntity * AlmGameScene::CreateEntity(const std::string & name)
{
	auto manager = const_cast<IAlmEntityManager*>(&Engine()->GetEntityManager());
	auto newEntity = manager->CreateEntity(name);

	return newEntity;
}

void AlmGameScene::ReleaseEntity(IAlmEntity* entity)
{
	auto manager = const_cast<IAlmEntityManager*>(&Engine()->GetEntityManager());
	manager->ReleaseEntity(entity);
}

IAlmEntity* AlmGameScene::FindByName(const std::string& name) const
{
	auto manager = const_cast<IAlmEntityManager*>(&Engine()->GetEntityManager());
	return manager->FindByName(name);
}