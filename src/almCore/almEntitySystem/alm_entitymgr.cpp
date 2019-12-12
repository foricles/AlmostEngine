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
}

AlmEntityManager::~AlmEntityManager()
{
}

IAlmEntity * AlmEntityManager::CreateEntity(const std::string &name)
{
	if (FindByName(name))
		return nullptr;

	AlmEntity *entity = new AlmEntity(this);
	entity->m_name = name;
	entity->m_id = GetHash(name);
	Push(entity);

	if (m_rootTransform != nullptr)
		entity->GetTransform()->AddChild(m_rootTransform);
	else
		m_rootTransform = entity->GetTransform();

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
	if (m_rootTransform)
	{
		m_rootTransform->UpdateModelMatrix();
	}
}

bool AlmEntityManager::Compare(const Node * left, const Node * right)
{
	return left->data->GetId() < right->data->GetId();
}

void AlmEntityManager::OnChangeParent(IAlmTransform * transform)
{
	if (transform == m_rootTransform)
		m_rootTransform = transform->GetParent();
}