#ifndef _ALM_RENDER_SYSTEM_INTERFACE_HPP_
#define _ALM_RENDER_SYSTEM_INTERFACE_HPP_

#include "alm_imaterial.hpp"
#include "alm_imesh.hpp"
#include "../src/almCore/alm_engsystem.hpp"
#include "../src/almCore/almEvents/alm_eventhandler.hpp"

namespace alme
{
class IAlmRenderSystem : public AlmEventHandler, protected AlmEngineSystem
{
public:
	IAlmRenderSystem(AlmostEngine *engine) : AlmEngineSystem(engine) {}

	virtual void				InitRenderAPIInstance() = 0;
	virtual void				OnWindowResize(unsigned int width, unsigned int height) = 0;

	virtual void				BeginRender() = 0;
	virtual void				FinishRender() = 0;

	virtual std::string			GetGpuVendorName() const = 0;
	virtual uint32_t			GetTotalVideoMemmory() const = 0;
	virtual uint32_t			GetAvailableVideoMemmory() const = 0;

	virtual IAlmMesh			*CreateMesh(uint32_t meshId) = 0;
	virtual IAlmRenderMaterial  *CreateMaterial(uint32_t materialId) = 0;
};

}


#endif // !_ALM_RENDER_SYSTEM_INTERFACE_HPP_