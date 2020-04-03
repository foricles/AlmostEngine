#ifndef _ALM_EVENT_HANDLER_HPP_
#define _ALM_EVENT_HANDLER_HPP_

#include "../src/almCore/alm_platform.hpp"

namespace alme
{
class IAlmDelegate;
class AlmEventHandlerRealize;
class AlmEventHandler
{
friend class IAlmDelegate;
public:
	AlmEventHandler();
	virtual ~AlmEventHandler();

private:
	void SetDelegate(IAlmDelegate *del);
	void RemoveDelegate(IAlmDelegate *del);

private:
	AlmEventHandlerRealize *m_impl;
};

}
#endif // !_ALM_EVENT_HANDLER_HPP_