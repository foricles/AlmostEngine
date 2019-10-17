#include "alm_idelegat.hpp"
#include "alm_eventhandler.hpp"

using namespace alme;

IAlmDelegate::~IAlmDelegate()
{

}

void IAlmDelegate::NotifyHandler(AlmEventHandler *hndl)
{
	hndl->SetDelegate(this);
}

void IAlmDelegate::OnDeleted(AlmEventHandler * hndl)
{
	hndl->RemoveDelegate(this);
}