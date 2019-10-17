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
	void OnWindowResize(unsigned int width, unsigned int height) override;


private:
	void InitInstance();
	void InitDevicePhys();
	void InitDeviceLogic();
	void InitCommandPool();
	void InitSurface();
	void InitSwapchain(unsigned int width, unsigned int height);

	void DestroyCommands();
	void CreateCommands();
	void SetupCommands();
};

}
#endif // !_ALM_VULKAN_RENDER_SYSTEM_HPP_