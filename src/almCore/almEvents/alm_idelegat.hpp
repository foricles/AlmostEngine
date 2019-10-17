#ifndef _ALM_DELEGATE_INTERFACE_HPP_
#define _ALM_DELEGATE_INTERFACE_HPP_

namespace alme
{

class IAlmDelegate
{
friend class AlmEventHandler;
public:
	virtual ~IAlmDelegate();

protected:
	void NotifyHandler(AlmEventHandler *hndl);
	virtual void OnDeleted(AlmEventHandler *hndl);
	virtual void RemoveHandler(AlmEventHandler *hndl) = 0;
};

}

#endif // !_ALM_DELEGATE_INTERFACE_HPP_