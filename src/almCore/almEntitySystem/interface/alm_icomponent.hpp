#ifndef _ALM_ICOMPONENT_HPP_
#define _ALM_ICOMPONENT_HPP_

#include <string>

namespace alme
{
	class AlmEntity;
	class IAlmComponent
	{
		friend class AlmEntity;
	public:
		IAlmComponent(AlmEntity* owner)
		 :m_entity(owner), m_name() {};
		virtual ~IAlmComponent() {};

		inline std::string&			GetName() { return m_name; }
		inline const std::string&	GetName() const { return m_name; }
		inline AlmEntity*			GetEntity() { return m_entity; }
		virtual void				OnNotify(IAlmComponent* component) {};

	protected:
		virtual void				SetName() = 0;

	private:
		std::string					m_name;
		AlmEntity*					m_entity;

	public:
		IAlmComponent(IAlmComponent&& rhv) = delete;
		IAlmComponent(const IAlmComponent& rhv) = delete;
		IAlmComponent& operator=(IAlmComponent&& rhv) = delete;
		IAlmComponent& operator=(const IAlmComponent& rhv) = delete;
	};
}

#endif // !_ALM_ICOMPONENT_HPP_
