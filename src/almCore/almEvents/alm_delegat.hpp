#ifndef _ALM_DELEGATE_HPP_
#define _ALM_DELEGATE_HPP_

#include "alm_idelegat.hpp"
#include "alm_eventhandler.hpp"
#include "../src/almCore/alm_platform.hpp"

template <int...>			struct int_sequence {};
template <int N, int... Is> struct make_int_sequence : make_int_sequence<N - 1, N - 1, Is...> {};
template <int... Is>		struct make_int_sequence<0, Is...> : int_sequence<Is...> {};

// begin with 0 here
template <int> struct placeholder_variadic {};

namespace std
{
	template <int N>
	struct is_placeholder<placeholder_variadic<N>> : integral_constant<int, N + 1> {};
}

namespace alme
{


template <class... Params>
class AlmDelegate : public IAlmDelegate
{
friend class AlmEventHandler;
using AlmCallback = std::function<void(Params &&...)>;
template<typename T> using AlmMemberCallback = std::function<void(T *, Params &&...)>;

struct AlmCallbackData
{
	AlmCallback func;
	bool isOnce;
	AlmCallbackData(AlmCallback foo, bool once)
		:func(foo), isOnce(once) 
	{}
};

using AlmHandlerPair = std::pair<AlmEventHandler*, std::vector<AlmCallbackData>>;

public:
	AlmDelegate() {};
	virtual ~AlmDelegate() 
	{
		for (AlmHandlerPair &pair : m_pool)
			OnDeleted(pair.first);
	};

	void Add(AlmEventHandler *hdl, AlmCallback callback, bool once = false) 
	{
		for (AlmHandlerPair &pair : m_pool)
		{
			if (pair.first == hdl)
			{
				pair.second.emplace_back(callback, once);
				return;
			}
		}

		std::vector<AlmCallbackData> data;
		data.emplace_back(callback, once);
		m_pool.push_back(AlmHandlerPair(hdl, data));
		NotifyHandler(hdl);
	};

	template <typename T> void Add(AlmEventHandler *hdl, AlmMemberCallback<T> callback, bool once = false) 
	{
		add<T>(static_cast<T*>(hdl), callback, once, make_int_sequence<sizeof...(Params)>{});
	};

	void Execute(Params... param)
	{
		for (AlmHandlerPair &pair : m_pool)
		{
			auto head = pair.second.begin();
			while (head != pair.second.end())
			{
				head->func(std::forward<Params>(param)...);
				head->isOnce ? head = pair.second.erase(head) : ++head; //TODO: need optimization
			}
		}
	}

private:
	std::vector<AlmHandlerPair> m_pool;

private:

	template <typename T, int... Is>
	inline void add(T *m, AlmMemberCallback<T> foo, bool once, int_sequence<Is...>)
	{
		Add(m, std::bind(foo, m, placeholder_variadic<Is>{}...));
	}

	void RemoveHandler(AlmEventHandler *hndl) override
	{
		auto pred = [hndl](const AlmHandlerPair &pair) -> bool { return pair.first == hndl; };
		auto fnd = std::find_if(m_pool.begin(), m_pool.end(), pred);
		if (fnd != m_pool.end()) m_pool.erase(fnd);
	}
};

}
#endif // !_ALM_DELEGATE_HPP_