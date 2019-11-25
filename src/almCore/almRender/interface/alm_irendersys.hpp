#ifndef _ALM_RENDER_SYSTEM_INTERFACE_HPP_
#define _ALM_RENDER_SYSTEM_INTERFACE_HPP_

#include "../src/almCore/almEvents/alm_eventhandler.hpp"
#include "alm_imaterial.hpp"

namespace alme
{

class IAlmRenderSystem : public AlmEventHandler
{
public:
	virtual ~IAlmRenderSystem() {};

	virtual void InitRenderAPIInstance() = 0;
	virtual void OnWindowResize(unsigned int width, unsigned int height) = 0;

	virtual void BeginRender() = 0;
	virtual void FinishRender() = 0;

	virtual IAlmRenderMaterial *CreateMaterial() = 0;
};

}


#endif // !_ALM_RENDER_SYSTEM_INTERFACE_HPP_