#include "alm_eventhandler.hpp"
#include "alm_idelegat.hpp"
#include <set>

namespace alme
{
	class AlmEventHandlerRealize
	{
	public:
		AlmEventHandlerRealize() {};
		~AlmEventHandlerRealize() {};

		std::set<IAlmDelegate*> m_delegates;
	};


	AlmEventHandler::AlmEventHandler()
	{
		m_impl = new AlmEventHandlerRealize();
	}

	AlmEventHandler::~AlmEventHandler()
	{
		for (IAlmDelegate *del : m_impl->m_delegates) del->RemoveHandler(this);
		delete m_impl;
	}


	void AlmEventHandler::SetDelegate(IAlmDelegate * del)
	{
		m_impl->m_delegates.insert(del);
	}

	void alme::AlmEventHandler::RemoveDelegate(IAlmDelegate * del)
	{
		m_impl->m_delegates.erase(del);
	}
}