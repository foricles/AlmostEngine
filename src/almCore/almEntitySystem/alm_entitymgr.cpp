#include "alm_entitymgr.hpp"
#include <xhash>
#include "../almEntitySystem/almEntity/alm_entity.hpp"

using namespace alme;


inline uint32_t GetHash(const std::string &str)
{
	return std::hash_value(str.c_str());
}


AlmEntityManager::AlmEntityManager()
{
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

bool AlmEntityManager::Compare(const Node * left, const Node * right)
{
	return left->data->GetId() < right->data->GetId();
}
