#ifndef _ALM_RENDER_SYSTEM_INTERFACE_HPP_
#define _ALM_RENDER_SYSTEM_INTERFACE_HPP_

#include "../src/almCore/almEvents/alm_eventhandler.hpp"

namespace alme
{

class IAlmRenderSystem : public AlmEventHandler
{
public:
	virtual ~IAlmRenderSystem() {};

	virtual void InitRenderAPIInstance() = 0;
	virtual void OnWindowResize(unsigned int width, unsigned int height) = 0;
};

}


#endif // !_ALM_RENDER_SYSTEM_INTERFACE_HPP_