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
		using comp_vector = std::vector<IAlmComponent*>;
		using comp_pair = std::pair<uint64_t, comp_vector>;
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
		IAlmComponent*					GetComponentByType(uint64_t typeId);
		comp_vector						GetComponentsByType(uint64_t typeId);

		template<class T> 
		void							CreateComponent(const std::string& tag = "");
		template<class T>
		std::vector<T*>					GetComponents();
		template<class T>
		IAlmComponent*					GetComponent();

	private:
		void							AddCoponent(IAlmComponent* component);

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
		std::vector<comp_pair>			m_components;
		std::vector<almTagPair>			m_componentsLayers;
	};

}

template<class T>
inline void alme::AlmEntity::CreateComponent(const std::string& tag)
{
	IAlmComponent* newComponent = new T(this);
	newComponent->SetName();
	AddCoponent(newComponent);
	if (tag.empty())
		return;
	AddComponentUnderTag(newComponent, tag);
}

template<class T>
inline std::vector<T*> alme::AlmEntity::GetComponents()
{
	const auto arr = GetComponentsByType(T::TypeID);
	std::vector<T*> ret(arr.size());
	for (auto& t : arr)
		ret.push_back(static_cast<T*>(t));
	return std::move(ret);
}

template<class T>
inline alme::IAlmComponent* alme::AlmEntity::GetComponent()
{
	const auto comp = GetComponentByType(T::TypeID);
	return comp ? static_cast<T*>(comp) : nullptr;
}

#endif // !_ALM_ENTITY_HPP_