#include "alm_entitymgr.hpp"
#include <xhash>
#include "../src/almCore/almEntitySystem/alm_entity.hpp"
#include "../src/almCore/almEntitySystem/alm_transform.hpp"

using namespace alme;


inline uint32_t GetHash(const std::string &str)
{
	return std::hash_value(str.c_str());
}


AlmEntityManager::AlmEntityManager(AlmostEngine *engine)
	: IAlmEntityManager(engine)
	, m_rootTransform(nullptr)
{
	auto entity = new AlmEntity();
	entity->m_name = "_alm_transformation_root_";
	entity->m_id = GetHash(entity->m_name);
	Push(entity);
	m_rootTransform = entity->GetTransform();
}

AlmEntityManager::~AlmEntityManager()
{
}

IAlmEntity * AlmEntityManager::CreateEntity(const std::string &name)
{
	if (FindByName(name))
		return nullptr;

	AlmEntity *entity = new AlmEntity();
	entity->m_name = name;
	entity->m_id = GetHash(name);
	Push(entity);

	m_rootTransform->AddChild(entity->GetTransform());

	return entity;
}

void AlmEntityManager::ReleaseEntity(IAlmEntity * entity)
{
	Delete(entity);
}

IAlmEntity * AlmEntityManager::FindByName(const std::string & name) const
{
	Node *head = m_root;
	uint32_t nameHash = GetHash(name);
	while (head)
	{
		if (head->data->GetName() == name) return head->data;
		head = (head->data->GetId() < nameHash) ? head->right : head->left;
	}
	return nullptr;
}

IAlmTransform * AlmEntityManager::GetRoot()
{
	return m_rootTransform;
}

void AlmEntityManager::UpdateTransformationTree()
{
	m_rootTransform->UpdateModelMatrix();
}

bool AlmEntityManager::Compare(const Node * left, const Node * right)
{
	return left->data->GetId() < right->data->GetId();
}