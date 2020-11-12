#ifndef _ALM_ICOMPONENT_HPP_
#define _ALM_ICOMPONENT_HPP_

#include <string>

namespace alme
{
	namespace comphelp
	{
		constexpr uint64_t get_pretty_hash(const char* p) noexcept
		{
			constexpr uint64_t A = 63689;
			constexpr uint64_t B = 378551;
			constexpr uint64_t C = 0x7FFFFFFFu;
			uint64_t a = A, hash = 0;
			while (*p++) { hash = hash * a + (*p); a = a * B; };
			return hash & C;
		}

		template<class T>
		constexpr const char* get_name() noexcept
		{
#if _MSC_VER
			return __FUNCSIG__;
#else
			return __PRETTY_FUNCTION__;
#endif
		}
	}

	class AlmEntity;
	class IAlmComponent
	{
		friend class AlmEntity;
	public:
		IAlmComponent(AlmEntity* owner)
		 :m_entity(owner), m_name("Component") {};
		virtual ~IAlmComponent() {};

		virtual uint64_t			GetTypeID() const noexcept = 0;
		inline std::string&			GetName() { return m_name; }
		inline const std::string&	GetName() const { return m_name; }
		inline AlmEntity*			GetEntity() { return m_entity; }
		virtual void				OnNotify(IAlmComponent* component) {};

	public:
		template<class T>
		inline static T* cast(IAlmComponent* component) noexcept {
			return (T::TypeID == component->GetID()) ? static_cast<T*>(component) : nullptr;
		}

	protected:
		virtual void				SetName() = 0;

	protected:
		std::string					m_name;
		AlmEntity*					m_entity;

	public:
		IAlmComponent(IAlmComponent&& rhv) = delete;
		IAlmComponent(const IAlmComponent& rhv) = delete;
		IAlmComponent& operator=(IAlmComponent&& rhv) = delete;
		IAlmComponent& operator=(const IAlmComponent& rhv) = delete;
	};

	template<class T>
	class IAlmComponentImp : public IAlmComponent
	{
	public:
		static constexpr uint64_t TypeID = comphelp::get_pretty_hash(comphelp::get_name<T>());
		IAlmComponentImp(AlmEntity* owner) : IAlmComponent(owner) {}
		uint64_t GetTypeID() const noexcept override final { return TypeID; }
	};
}

#endif // !_ALM_ICOMPONENT_HPP_
