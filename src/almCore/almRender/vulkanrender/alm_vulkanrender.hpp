#ifndef _ALM_VULKAN_RENDER_SYSTEM_HPP_
#define _ALM_VULKAN_RENDER_SYSTEM_HPP_

#include "../src/almCore/almRender/interface/alm_irendersys.hpp"

namespace alme
{

struct sAlmVulkanContext;
class AlmVulkanRender : public IAlmRenderSystem
{
public:
	AlmVulkanRender();
	~AlmVulkanRender();

	void InitRenderAPIInstance() override;
	void OnWindowResize(unsigned int width, unsigned int height) override;

	void BeginRender() override;
	void FinishRender() override;

	IAlmRenderMaterial *CreateMaterial() override;

private:
	void InitInstance();
	void InitPhysDevice();
	void InitLogicalDevice();
	void InitRenderSurface();

	void CreateSwapchain(unsigned int width, unsigned int height);
	void CreateRenderpass();
	void CreatePipeline();
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateSynchronization();

	void DeleteSynchronization();
	void DeleteCommandPool();
	void DeleteFramebuffers();
	void DeletePipeline();
	void DeleteRenderpass();
	void DeleteSwapchain();

private:
	sAlmVulkanContext *m_variables;
};

}
#endif // !_ALM_VULKAN_RENDER_SYSTEM_HPP_