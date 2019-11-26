#ifndef _ALM_VULKAN_RENDER_INSTANCE_HPP_
#define _ALM_VULKAN_RENDER_INSTANCE_HPP_

#include <vulkan/vulkan.hpp>

namespace alme
{
	struct sAlmVulkanContext
	{
		vk::Instance instance;

		vk::Device device;
		vk::PhysicalDevice physicalDevice;

		uint32_t graphicsFamilyIndex;
		vk::Queue graphicsQueue;

		vk::SurfaceKHR surface;

		vk::SwapchainKHR swapChain;
		std::vector<vk::Image> swapChainImages;
		vk::Format swapChainImageFormat;
		vk::Extent2D swapChainExtent;
		std::vector<vk::ImageView> swapChainImageViews;

		vk::RenderPass renderPass;

		std::vector<vk::Framebuffer> swapChainFramebuffers;

		vk::CommandPool commandPool;
		std::vector<vk::CommandBuffer> commandBuffers;

		std::vector<vk::Semaphore> imageAvailableSemaphore;
		std::vector<vk::Semaphore> renderFinishedSemaphore;
		uint32_t currentFrame;
	};
}

#endif // !_ALM_VULKAN_RENDER_INSTANCE_HPP_