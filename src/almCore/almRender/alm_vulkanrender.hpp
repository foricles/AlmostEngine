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

	void BeginRender() override {}
	void FinishRender() override {}

private:
	void InitInstance();
	void InitDevicePhys();
	void InitDeviceLogic();
	void InitSurface();
	void InitCommandPool();
	void InitSynch();

	void DestroyCommands();
	void CreateCommands();
	void SetupCommands();
	void SetupSwapchain(unsigned int width, unsigned int height);
};

}
#endif // !_ALM_VULKAN_RENDER_SYSTEM_HPP_