#ifndef _ALM_VULKAN_RENDER_SYSTEM_HPP_
#define _ALM_VULKAN_RENDER_SYSTEM_HPP_

#include "interface/alm_irendersys.hpp"

namespace alme
{

class AlmVulkanRender : public IAlmRenderSystem
{
public:
	AlmVulkanRender();
	~AlmVulkanRender();

	void InitRenderAPIInstance() override;

private:
	void InitInstance();
	void InitDevicePhys();
	void InitDeviceLogic();
	void InitCommandPool();
	void InitSurface();
	void InitSwapchain();

};

}
#endif // !_ALM_VULKAN_RENDER_SYSTEM_HPP_