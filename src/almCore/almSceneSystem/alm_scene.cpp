#include "alm_scene.hpp"
#include "../src/alm_engine.hpp"
#include "../src/almCore/almUser/almWindow/alm_iwindow.hpp"
#include "../src/almCore/almEntitySystem/interface/alm_imgr.hpp"
#include "../src/almCore/almRender/interface/alm_irendersys.hpp"
#include "../src/almCore/almEntitySystem/alm_entity.hpp"
#include "../src/almCore/almEntitySystem/alm_transform.hpp"

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
	m_rootTransform = manager->CreateEntity(ROOT_TANSFORM_NAME);
}

const AlmostEngine * AlmGameScene::Engine() const
{
	return m_engine;
}

AlmEntity * AlmGameScene::CreateEntity(const std::string & name)
{
	return CreateEntity(name, m_rootTransform);
}

AlmEntity* AlmGameScene::CreateEntity(const std::string& name, AlmTransform* parent)
{
	auto manager = const_cast<IAlmEntityManager*>(&Engine()->GetEntityManager());
	auto newEntity = manager->CreateEntity(name);
	parent->AddChild(newEntity);
	return newEntity;
}

void AlmGameScene::ReleaseEntity(AlmEntity* entity)
{
	auto manager = const_cast<IAlmEntityManager*>(&Engine()->GetEntityManager());
	if (AlmTransform * parent = entity->GetParent())
		parent->RemoveChild(entity);
	manager->ReleaseEntity(entity);
}

AlmEntity* AlmGameScene::FindByName(const std::string& name) const
{
	auto manager = const_cast<IAlmEntityManager*>(&Engine()->GetEntityManager());
	return manager->FindByName(name);
}

AlmTransform* AlmGameScene::GetRoot()
{
	return m_rootTransform;
}

void AlmGameScene::UpdateTransformationTree()
{
	m_rootTransform->UpdateModelMatrix();
}
