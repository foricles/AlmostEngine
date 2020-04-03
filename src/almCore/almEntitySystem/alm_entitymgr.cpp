#include "alm_entitymgr.hpp"
#include <xhash>
#include "../src/almCore/almEntitySystem/alm_entity.hpp"
#include "../src/almCore/almEntitySystem/alm_transform.hpp"

using namespace alme;

#define ROOT_TANSFORM_NAME "_alm_transformation_root_"

inline uint32_t GetHash(const std::string &str)
{
	std::hash<const char*> ass;
	return ass._Do_hash(str.c_str());
}


AlmEntityManager::AlmEntityManager(AlmostEngine *engine)
	: IAlmEntityManager(engine)
	, m_rootTransform(nullptr)
{
	auto entity = new AlmEntity();
	entity->m_name = ROOT_TANSFORM_NAME;
	entity->m_id = GetHash(entity->m_name);
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
	entity->onDelete.Execute(entity);
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

void alme::AlmEntityManager::ReleaseAllEntities()
{
	if (m_root) ReleaseTree(m_root);
}

void alme::AlmEntityManager::UpdateAllEntities()
{
	if (m_root) UpdateAllEntities(m_root);
}

bool AlmEntityManager::Compare(const Node * left, const Node * right)
{
	return left->data->GetId() < right->data->GetId();
}

void alme::AlmEntityManager::UpdateAllEntities(Node* root)
{
	if (root->left) UpdateAllEntities(root->left);
	if (root->right) UpdateAllEntities(root->right);
	if (root->data->m_hasUpdate)
		root->data->onUpdate.Execute(root->data, 0);
}