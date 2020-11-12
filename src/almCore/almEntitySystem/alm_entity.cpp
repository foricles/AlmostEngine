#include "alm_entity.hpp"
#include "../src/almCore/almComponents/alm_icomponent.hpp"
#include <algorithm>


__forceinline alme::AlmEntity::almTagPair* FindComponentLayout(std::vector<alme::AlmEntity::almTagPair>& pairs, const std::string& tag);

namespace alme
{

	AlmEntity::AlmEntity()
		: m_id(0)
		, m_name("Entity")
		, m_hasUpdate(false)
	{
	}

	AlmEntity::~AlmEntity()
	{
	}


	bool AlmEntity::operator<(const AlmEntity& rhv) const
	{
		return GetId() < rhv.GetId();
	}

	bool AlmEntity::operator==(const AlmEntity& rhv) const
	{
		return GetId() == rhv.GetId();
	}

	const uint32_t AlmEntity::GetId() const
	{
		return m_id;
	}

	const std::string& AlmEntity::GetName() const
	{
		return m_name;
	}


	void AlmEntity::AddComponentTag(const std::string& tag)
	{
		if (HasComponentTag(tag))
			return;
		m_componentsLayers.resize(m_componentsLayers.size() + 1);
		m_componentsLayers.back().first = tag;
	}

	bool AlmEntity::HasComponentTag(const std::string& tag)
	{
		for (const auto& t : m_componentsLayers)
			if (tag == t.first)
				return true;
		return false;
	}

	void AlmEntity::DeleteComponentTag(const std::string& tag)
	{
		const auto fnd = std::find_if(
			m_componentsLayers.begin(),
			m_componentsLayers.end(),
			[&tag](const almTagPair& pair)->bool {
				return tag == pair.first;
			}
		);

		if (fnd != m_componentsLayers.end())
			m_componentsLayers.erase(fnd);
	}

	void AlmEntity::NotifyAllComponents(IAlmComponent* component)
	{
		for (const auto& pair : m_components)
			for (const auto& t : pair.second)
				if (t != component)
					t->OnNotify(component);
	}

	void AlmEntity::NotifyComponentsByTag(const std::string& tag, IAlmComponent* component)
	{
		if (almTagPair* components = FindComponentLayout(m_componentsLayers, tag)) {
			for (const auto& t : components->second) {
				if (t != component)
					t->OnNotify(component);
			}
		}
	}

	void AlmEntity::NotifyComponentsByName(const std::string& name, IAlmComponent* component)
	{
		for (auto& pair : m_components)
		{
			auto fnd = std::find_if(
				pair.second.begin(),
				pair.second.end(),
				[&name](const IAlmComponent* comp)->bool {
					return name == comp->GetName();
				}
			);
			if (fnd != pair.second.end())
				(*fnd)->OnNotify(component);
		}
	}

	void AlmEntity::AddComponentUnderTag(const std::string& tag, IAlmComponent* component)
	{
		if (almTagPair* components = FindComponentLayout(m_componentsLayers, tag)) {
			const auto fnd = std::find(components->second.begin(), components->second.end(), component);
			if (fnd == components->second.end())
				components->second.push_back(component);
		}
	}

	void AlmEntity::RemoveComponentFromTag(const std::string& tag, IAlmComponent* component)
	{
		if (almTagPair* components = FindComponentLayout(m_componentsLayers, tag)) {
			const auto fnd = std::find(components->second.begin(), components->second.end(), component);
			if (fnd != components->second.end())
				components->second.erase(fnd);
		}
	}

	AlmEntity::comp_vector AlmEntity::GetComponentsByType(uint64_t typeId)
	{
		comp_vector ret;
		auto fnd = std::find_if(
			m_components.begin(),
			m_components.end(),
			[typeId](const comp_pair& p)->bool { return p.first == typeId; }
		);
		if (fnd != m_components.end())
			ret = fnd->second;
		return std::move(ret);
	}

	IAlmComponent* AlmEntity::GetComponentByType(uint64_t typeId)
	{
		auto fnd = std::find_if(
			m_components.begin(),
			m_components.end(),
			[typeId](const comp_pair& p)->bool { return p.first == typeId; }
		);
		if (fnd != m_components.end()) {
			if (fnd->second.size() > 0)
				return fnd->second[0];
		}
		return nullptr;
	}

	void AlmEntity::AddCoponent(IAlmComponent* component)
	{
		component->m_entity = this;
		auto typeId = component->GetTypeID();
		auto fnd = std::find_if(
			m_components.begin(),
			m_components.end(),
			[typeId](const comp_pair& p)->bool { return p.first == typeId; }
		);
		if (fnd == m_components.end())
		{
			const comp_vector vec = { component };
			m_components.emplace_back(typeId, vec);
			std::sort(m_components.begin(), m_components.end());
			return;
		}
		fnd->second.push_back(component);
	}

	AlmEntity* AlmEntity::GetCopy()
	{
		return nullptr;
	}
}



alme::AlmEntity::almTagPair* FindComponentLayout(std::vector<alme::AlmEntity::almTagPair>& pairs, const std::string& tag)
{
	const auto fnd = std::find_if(pairs.begin(), pairs.end(),
		[&tag](const alme::AlmEntity::almTagPair& pair)->bool {
			return tag == pair.first;
		}
	);
	if (fnd != pairs.end())
		return &(*fnd);
	return nullptr;
}
