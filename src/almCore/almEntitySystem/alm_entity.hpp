#ifndef _ALM_ENTITY_HPP_
#define _ALM_ENTITY_HPP_

#include "alm_transform.hpp"
#include "../almEvents/alm_delegat.hpp"

namespace alme
{
	class IAlmComponent;
	class AlmEntity : public AlmTransform
	{
		friend class AlmEntityManager;
	public:
		using almTagPair = std::pair<std::string, std::vector<IAlmComponent*>>;


		AlmEntity();
		AlmEntity(const AlmEntity &rhv) = delete;
		AlmEntity & operator=(const AlmEntity &rhv) = delete;
		~AlmEntity();

		const uint32_t					GetId() const;
		const std::string &				GetName() const;

		void							AddComponentTag(const std::string &tag);
		bool							HasComponentTag(const std::string &tag);
		void							DeleteComponentTag(const std::string &tag);
		void							NotifyAllComponents(IAlmComponent* component);
		void							NotifyComponentsByTag(const std::string &tag, IAlmComponent* component);
		void							NotifyComponentsByName(const std::string &name, IAlmComponent* component);
		void							AddComponentUnderTag(const std::string& tag, IAlmComponent* component);
		void							RemoveComponentFromTag(const std::string& tag, IAlmComponent* component);
		template<class T> void			CreateComponent(const std::string& tag = "");

	protected:
		AlmEntity*						GetCopy();

	public:
		bool operator < (const AlmEntity &rhv) const;
		bool operator == (const AlmEntity &rhv) const;

		AlmDelegate<AlmEntity*>			onDelete;
		AlmDelegate<AlmEntity*>			onUpdate;

	private:
		uint32_t						m_id;
		std::string						m_name;
		bool							m_hasUpdate;

	private:
		std::vector<IAlmComponent*>		m_components;
		std::vector<almTagPair>			m_componentsLayers;
	};

}

template<class T>
inline void alme::AlmEntity::CreateComponent(const std::string& tag)
{
	IAlmComponent* newComponent = new T(this);
	newComponent->SetName();
	m_components.push_back(newComponent);
	if(!tag.empty())
		AddComponentUnderTag(newComponent, tag);
}

#endif // !_ALM_ENTITY_HPP_