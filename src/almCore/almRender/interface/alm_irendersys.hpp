#ifndef _ALM_RENDER_SYSTEM_INTERFACE_HPP_
#define _ALM_RENDER_SYSTEM_INTERFACE_HPP_

namespace alme
{

class IAlmRenderSystem
{
public:
	virtual ~IAlmRenderSystem() {};

	virtual void InitRenderAPIInstance() = 0;
};

}


#endif // !_ALM_RENDER_SYSTEM_INTERFACE_HPP_