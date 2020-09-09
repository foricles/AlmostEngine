#include "alm_entitymgr.hpp"
#include "../src/almCore/almEntitySystem/alm_entity.hpp"
#include "../src/almCore/almEntitySystem/alm_transform.hpp"
#include <map>

namespace alme
{
	inline uint32_t GetHash(const std::string& str) {
		return std::hash<std::string>{}(str);
	}

	class AlmEntityManager::AlmContainer
	{
	public:
		std::map<uint32_t, AlmEntity*> map;
		void push(AlmEntity* e) {map.emplace(e->GetId(), e);}
		AlmEntity* kill(AlmEntity* e) { 
			auto fnd = map.find(e->GetId());
			if (fnd == map.end()) return nullptr;
			map.erase(fnd);
			return fnd->second;
		}
	};
}


using namespace alme;


AlmEntityManager::AlmEntityManager(AlmostEngine *engine)
	: IAlmEntityManager(engine)
	, m_container(nullptr)
	, m_memmory(0)
{
	m_container = new AlmContainer();
}

AlmEntityManager::~AlmEntityManager()
{
	delete m_container;
}

AlmEntity * AlmEntityManager::CreateEntity(const std::string &name)
{
	if (FindByName(name))
		return nullptr;

	AlmEntity* entity = new AlmEntity();
	entity->m_name = name;
	entity->m_id = GetHash(name);
	m_memmory += sizeof(AlmEntity);

	m_container->push(entity);
	return entity;
}

void AlmEntityManager::ReleaseEntity(AlmEntity * entity)
{
	if (auto e = m_container->kill(entity))
	{
		e->onDelete.Execute(entity);
		delete entity;
		m_memmory -= sizeof(AlmEntity);
	}
}

AlmEntity * AlmEntityManager::FindByName(const std::string & name) const
{
	auto fnd = m_container->map.find(GetHash(name));
	if (fnd == m_container->map.end()) return nullptr;
	return fnd->second;
}

void AlmEntityManager::ReleaseAllEntities()
{
	auto beg = m_container->map.begin();
	while (beg != m_container->map.end())
	{
		auto sec = beg->second;
		sec->onDelete.Execute(sec);
		delete beg->second;
		beg->second = nullptr;
		m_container->map.erase(beg);
	}
}

void AlmEntityManager::UpdateAllEntities()
{
	for (const auto & n : m_container->map)
		if (n.second->m_hasUpdate)
			n.second->onUpdate.Execute(n.second);
}

uint32_t AlmEntityManager::AllocatedMemory() const
{
	return m_memmory;
}

uint32_t AlmEntityManager::EntitiesCount() const
{
	return m_container->map.size();
}

uint32_t AlmEntityManager::EntitiesCountWithUpdate() const
{
	uint32_t count = 0;
	for (const auto& n : m_container->map)
		count += static_cast<bool>(n.second->m_hasUpdate);
	return count;
}