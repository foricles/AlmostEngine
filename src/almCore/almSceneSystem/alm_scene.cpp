#include "alm_scene.hpp"
#include "../src/alm_engine.hpp"
#include "../src/almCore/almUser/almWindow/alm_iwindow.hpp"
#include "../src/almCore/almEntitySystem/interface/alm_imgr.hpp"
#include "../src/almCore/almRender/interface/alm_irendersys.hpp"
#include "../src/almCore/almEntitySystem/interface/alm_ientity.hpp"
#include "../src/almCore/almEntitySystem/interface/alm_itransform.hpp"

#define ROOT_TANSFORM_NAME "_alm_transformation_root_"

using namespace alme;

AlmGameScene::AlmGameScene()
	: m_engine(nullptr)
	, m_rootTransform(nullptr)
{

}

AlmGameScene::~AlmGameScene()
{

}

void AlmGameScene::SetEnginePtr(AlmostEngine * engine)
{
	m_engine = engine;
	auto manager = const_cast<IAlmEntityManager*>(&engine->GetEntityManager());
	m_rootTransform = manager->CreateEntity(ROOT_TANSFORM_NAME)->GetTransform();
}

const AlmostEngine * AlmGameScene::Engine() const
{
	return m_engine;
}

IAlmEntity * AlmGameScene::CreateEntity(const std::string & name)
{
	return CreateEntity(name, m_rootTransform);
}

IAlmEntity* AlmGameScene::CreateEntity(const std::string& name, IAlmTransform* parent)
{
	auto manager = const_cast<IAlmEntityManager*>(&Engine()->GetEntityManager());
	auto newEntity = manager->CreateEntity(name);
	parent->AddChild(newEntity->GetTransform());
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

IAlmTransform* AlmGameScene::GetRoot()
{
	return m_rootTransform;
}

void AlmGameScene::UpdateTransformationTree()
{
	m_rootTransform->UpdateModelMatrix();
}
