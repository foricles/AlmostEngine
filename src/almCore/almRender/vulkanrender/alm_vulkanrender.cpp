#include "alm_vulkanrender.hpp"
#include "../src/almCore/almRender/vulkanrender/alm_vkinstance.hpp"
#include "../src/almCore/alm_log.hpp"
#include "../src/almCore/alm_platform.hpp"
#include "../src/almCore/almRender/vulkanrender/vkmaterial/alm_vkmaterial.hpp"

#include <string>
#include <vector>
#include <array>
#if _DEBUG
	static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			ALM_LOG_ERROR(pCallbackData->pMessage);
		}
		else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			ALM_LOG_WARNING(pCallbackData->pMessage);
		}

		return VK_FALSE;
	}
#endif

using namespace alme;

AlmVulkanRender::AlmVulkanRender(AlmostEngine *engine)
	: IAlmRenderSystem(engine)
{
	m_variables = new sAlmVulkanContext();
	m_variables->currentFrame = 0;
	ALM_LOG_TEXT("sAlmVulkanVariables: ", sizeof(sAlmVulkanContext));
}

AlmVulkanRender::~AlmVulkanRender()
{
	delete m_variables;
}

uint32_t AlmVulkanRender::GetTotalVideoMemmory() const
{
	return m_variables->physicalDevice.getProperties().limits.maxMemoryAllocationCount;
};

uint32_t AlmVulkanRender::GetAvailableVideoMemmory() const
{
	return 0;
};

std::string	AlmVulkanRender::GetGpuVendorName() const
{
	return m_variables->physicalDevice.getProperties().deviceName;
}

void AlmVulkanRender::InitRenderAPIInstance()
{
	InitInstance();
	InitPhysDevice();
	InitLogicalDevice();
	InitRenderSurface();

#if defined(VK_USE_PLATFORM_WIN32_KHR)
	RECT rect;
	if (GetWindowRect(GetActiveWindow(), &rect))
	{
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		CreateSwapchain(width, height);
	}
#endif

	CreateRenderpass();
	CreateFramebuffers();
	CreateCommandPool();
	CreateSynchronization();
}

void AlmVulkanRender::OnWindowResize(unsigned int width, unsigned int height)
{
	m_variables->device.waitIdle();

	DeleteSynchronization();
	DeleteCommandPool();
	DeleteFramebuffers();
	DeleteRenderpass();
	DeleteSwapchain();

	CreateSwapchain(width, height);
	CreateRenderpass();
	CreateFramebuffers();
	CreateCommandPool();
	CreateSynchronization();
}



void AlmVulkanRender::BeginRender()
{
	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);

	vk::ClearColorValue clearColor;
	vk::ClearValue clearValue;
	clearValue.color = clearColor;

	vk::ImageSubresourceRange imageRange;
	imageRange.setLayerCount(1).setLevelCount(1);
	imageRange.setAspectMask(vk::ImageAspectFlagBits::eColor);

	for (uint32_t i(0); i < m_variables->swapChainImages.size(); ++i) 
	{
		m_variables->commandBuffers[i].begin(beginInfo);
		m_variables->commandBuffers[i].clearColorImage(m_variables->swapChainImages[i], vk::ImageLayout::eTransferDstOptimal, &clearColor, 1, &imageRange);
	}

}

void AlmVulkanRender::FinishRender()
{
	for (uint32_t i(0); i < m_variables->swapChainImages.size(); ++i)
	{
		m_variables->commandBuffers[i].draw(3, 1, 0, 0);
		m_variables->commandBuffers[i].end();
	}
	
	vk::Semaphore &image = m_variables->imageAvailableSemaphore[m_variables->currentFrame];
	vk::Semaphore &render = m_variables->renderFinishedSemaphore[m_variables->currentFrame];
	uint32_t imageIndex;
	vk::Result result = m_variables->device.acquireNextImageKHR(m_variables->swapChain, UINT64_MAX, image, VK_NULL_HANDLE, &imageIndex);

	if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
	{
		OnWindowResize(m_variables->swapChainExtent.width, m_variables->swapChainExtent.height);
		return;
	}

	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	vk::SubmitInfo submitInfo;
	submitInfo.setCommandBufferCount(1);
	submitInfo.setPCommandBuffers(&m_variables->commandBuffers[imageIndex]);
	submitInfo.setSignalSemaphoreCount(1);
	submitInfo.setPSignalSemaphores(&render);
	submitInfo.setPWaitDstStageMask(waitStages);
	submitInfo.setWaitSemaphoreCount(1);
	submitInfo.setPWaitSemaphores(&image);

	m_variables->device.waitIdle();
	result = m_variables->graphicsQueue.submit(1, &submitInfo, VK_NULL_HANDLE);
	if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
	{
		OnWindowResize(m_variables->swapChainExtent.width, m_variables->swapChainExtent.height);
		return;
	}
	m_variables->graphicsQueue.waitIdle();

	vk::PresentInfoKHR presentInfo;
	presentInfo.setSwapchainCount(1);
	presentInfo.setPSwapchains(&m_variables->swapChain);
	presentInfo.setPImageIndices(&imageIndex);
	submitInfo.setWaitSemaphoreCount(1);
	submitInfo.setPWaitSemaphores(&render);

	result = m_variables->graphicsQueue.presentKHR(&presentInfo);
	if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
	{
		OnWindowResize(m_variables->swapChainExtent.width, m_variables->swapChainExtent.height);
		return;
	}

	m_variables->device.waitIdle();
	ALM_LOG_INFO("finish_render");
	m_variables->currentFrame = (imageIndex + 1) % 2;
}



IAlmMesh * AlmVulkanRender::CreateMesh(uint32_t meshId)
{
	return nullptr;
}

IAlmRenderMaterial * AlmVulkanRender::CreateMaterial(uint32_t materialId)
{
	//TODO: write manager
	return new AlmVkMaterial(m_variables);
}



void AlmVulkanRender::InitInstance()
{
	std::vector<vk::ExtensionProperties> installedExtensions = vk::enumerateInstanceExtensionProperties().value;
	std::vector<const char*> wantedExtensions =
	{
		VK_KHR_SURFACE_EXTENSION_NAME
#if defined(VK_USE_PLATFORM_WIN32_KHR)
		, VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
		, VK_MVK_MACOS_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XCB_KHR)
		, VK_KHR_XCB_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
		, VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
		, VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XCB_KHR)
		, VK_KHR_XCB_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
		, VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_MIR_KHR) || defined(VK_USE_PLATFORM_DISPLAY_KHR)
		, VK_KHR_DISPLAY_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
		, VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_IOS_MVK)
		, VK_MVK_IOS_SURFACE_EXTENSION_NAME
#endif

#if _DEBUG
		, VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
	};

	auto ext = wantedExtensions.begin();
	while (ext != wantedExtensions.end())
	{
		auto cmp = [ext](const vk::ExtensionProperties &prop) ->bool { return std::strcmp(prop.extensionName, (*ext)) == 0; };
		auto fnd = std::find_if(installedExtensions.begin(), installedExtensions.end(), cmp);
		if (fnd == installedExtensions.end())
		{
			ALM_LOG_WARNING("Extension", (*ext), "not supported and will be removed");
			ext = wantedExtensions.erase(ext);
		}
		else
		{
			++ext;
		}
	}

	vk::ApplicationInfo appInfo;
	appInfo.setApiVersion(VK_MAKE_VERSION(1, 0, 2));
	appInfo.setEngineVersion(VK_MAKE_VERSION(0, 1, 0));
	appInfo.setPEngineName("AlmostEngine");
	appInfo.setPApplicationName("");

	vk::InstanceCreateInfo instanceInfo;
	instanceInfo.setPApplicationInfo(&appInfo);
	instanceInfo.setEnabledExtensionCount(wantedExtensions.size());
	instanceInfo.setPpEnabledExtensionNames(wantedExtensions.data());
	instanceInfo.setEnabledLayerCount(0);
	instanceInfo.setPpEnabledLayerNames(nullptr);

#if _DEBUG
	std::vector<vk::LayerProperties> installedLayers = vk::enumerateInstanceLayerProperties().value;
	std::vector<const char*> validationLayers =
	{
		"VK_LAYER_KHRONOS_validation",
		"VK_LAYER_LUNARG_standard_validation"
	};
	auto layer = validationLayers.begin();
	while(layer != validationLayers.end())
	{
		auto cmp = [layer](const vk::LayerProperties &prop) ->bool { return std::strcmp(prop.layerName, (*layer)) == 0; };
		auto fnd = std::find_if(installedLayers.begin(), installedLayers.end(), cmp);
		if (fnd == installedLayers.end())
		{
			ALM_LOG_WARNING("Layer", (*layer), "not supported and will be removed");
			layer = validationLayers.erase(layer);
		}
		else
		{
			++layer;
		}
	}

	instanceInfo.setEnabledLayerCount(validationLayers.size());
	instanceInfo.setPpEnabledLayerNames(validationLayers.data());
#endif



	m_variables->instance = vk::createInstance(instanceInfo).value;



#if _DEBUG
	vk::DebugUtilsMessengerCreateInfoEXT debugInfo;
	debugInfo.setMessageSeverity(
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo
	);
	debugInfo.setMessageType(
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
		vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
		vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
	);
	debugInfo.setPfnUserCallback(vulkanDebugCallback);

	VkDebugUtilsMessengerEXT debugMessenger;
	auto instance = (VkInstance)m_variables->instance;
	auto debInf = (VkDebugUtilsMessengerCreateInfoEXT)debugInfo;
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		VkResult ttt = func(instance, &debInf, nullptr, &debugMessenger);
		if (ttt != VK_SUCCESS) ALM_LOG_ERROR("Failded to create DebugCallback");
	}
	else ALM_LOG_ERROR("Failded to create DebugCallback");
#endif
}

void AlmVulkanRender::InitPhysDevice()
{
	std::vector<vk::PhysicalDevice> devices = m_variables->instance.enumeratePhysicalDevices().value;
	m_variables->physicalDevice = devices[0];
	for (const vk::PhysicalDevice &device : devices)
	{
		auto & props = device.getProperties();
		if (props.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
		{
			ALM_LOG_INFO("Selected GPU: ", props.deviceName);
			m_variables->physicalDevice = device;
			break;
		}
	}

	m_variables->graphicsFamilyIndex = 0;
	std::vector<vk::QueueFamilyProperties> queueProps = m_variables->physicalDevice.getQueueFamilyProperties();
	for (const vk::QueueFamilyProperties &prop : queueProps)
	{
		if (prop.queueFlags & vk::QueueFlagBits::eGraphics)
			break;
		++m_variables->graphicsFamilyIndex;
	}
}

void AlmVulkanRender::InitLogicalDevice()
{
	float priority = 1;
	vk::DeviceQueueCreateInfo queueInfo;
	queueInfo.setQueueCount(1);
	queueInfo.setPQueuePriorities(&priority);
	queueInfo.setQueueFamilyIndex(m_variables->graphicsFamilyIndex);

	vk::PhysicalDeviceFeatures deviceFeatures;

	std::vector<const char*> deviceExtensions = 
	{
		VK_NV_GLSL_SHADER_EXTENSION_NAME,
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	auto instaledExt = m_variables->physicalDevice.enumerateDeviceExtensionProperties().value;

	auto ext = deviceExtensions.begin();
	while (ext != deviceExtensions.end())
	{
		auto cmp = [ext](const vk::ExtensionProperties &prop) ->bool { return std::strcmp(prop.extensionName, (*ext)) == 0; };
		auto fnd = std::find_if(instaledExt.begin(), instaledExt.end(), cmp);
		if (fnd == instaledExt.end())
		{
			ALM_LOG_WARNING("Extencion", (*ext), "not supported and will be removed");
			ext = deviceExtensions.erase(ext);
		}
		else
		{
			++ext;
		}
	}

	vk::DeviceCreateInfo deviceInfo;
	deviceInfo.setQueueCreateInfoCount(1);
	deviceInfo.setPQueueCreateInfos(&queueInfo);
	deviceInfo.setPEnabledFeatures(&deviceFeatures);
	deviceInfo.setEnabledExtensionCount(deviceExtensions.size());
	deviceInfo.setPpEnabledExtensionNames(deviceExtensions.data());

	m_variables->device = m_variables->physicalDevice.createDevice(deviceInfo).value;
	m_variables->graphicsQueue = m_variables->device.getQueue(m_variables->graphicsFamilyIndex, 0);
}

void AlmVulkanRender::InitRenderSurface()
{
#if defined(VK_USE_PLATFORM_WIN32_KHR)

	vk::Win32SurfaceCreateInfoKHR surfaceInfo;
	surfaceInfo.setHinstance(GetModuleHandle(nullptr));
	surfaceInfo.setHwnd(GetActiveWindow());

	m_variables->surface = m_variables->instance.createWin32SurfaceKHR(surfaceInfo).value;
	vk::Bool32 isSuport = m_variables->physicalDevice.getSurfaceSupportKHR(m_variables->graphicsFamilyIndex, m_variables->surface).value;
	ALM_LOG_ASSERT(isSuport, "Render surface not supported");

#endif
}

void AlmVulkanRender::CreateSwapchain(unsigned int width, unsigned int height)
{
	vk::SurfaceCapabilitiesKHR capabilities = m_variables->physicalDevice.getSurfaceCapabilitiesKHR(m_variables->surface).value;
	std::vector<vk::SurfaceFormatKHR> formats = m_variables->physicalDevice.getSurfaceFormatsKHR(m_variables->surface).value;
	std::vector<vk::PresentModeKHR> presentModes = m_variables->physicalDevice.getSurfacePresentModesKHR(m_variables->surface).value;

	vk::SwapchainCreateInfoKHR swapchainInfo;
	swapchainInfo.setMinImageCount(2);
	swapchainInfo.setSurface(m_variables->surface);
	swapchainInfo.setImageFormat(formats[0].format);
	swapchainInfo.setImageColorSpace(formats[0].colorSpace);
	swapchainInfo.setImageExtent(capabilities.currentExtent);
	swapchainInfo.setImageArrayLayers(1);
	swapchainInfo.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst);
	swapchainInfo.setPreTransform(capabilities.currentTransform);
	swapchainInfo.setImageSharingMode(vk::SharingMode::eExclusive);
	swapchainInfo.setPQueueFamilyIndices(nullptr);
	swapchainInfo.setQueueFamilyIndexCount(0);
	swapchainInfo.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);
	swapchainInfo.setClipped(true);
	swapchainInfo.setPresentMode(vk::PresentModeKHR::eMailbox);

	m_variables->swapChain = m_variables->device.createSwapchainKHR(swapchainInfo).value;

	m_variables->swapChainImages = m_variables->device.getSwapchainImagesKHR(m_variables->swapChain).value;
	m_variables->swapChainExtent = capabilities.currentExtent;
	m_variables->swapChainImageFormat = formats[0].format;

	m_variables->swapChainImageViews.reserve(m_variables->swapChainImages.size());
	for (auto & image : m_variables->swapChainImages)
	{
		vk::ImageSubresourceRange subresRange;
		subresRange.setAspectMask(vk::ImageAspectFlagBits::eColor);
		subresRange.setBaseMipLevel(0);
		subresRange.setBaseArrayLayer(0);
		subresRange.setLayerCount(1);
		subresRange.setLevelCount(1);

		vk::ImageViewCreateInfo imageViewInfo;
		imageViewInfo.setImage(image);
		imageViewInfo.setViewType(vk::ImageViewType::e2D);
		imageViewInfo.setFormat(m_variables->swapChainImageFormat);
		imageViewInfo.setComponents(vk::ComponentMapping()); // VK_COMPONENT_SWIZZLE_IDENTITY
		imageViewInfo.setSubresourceRange(subresRange);

		m_variables->swapChainImageViews.push_back(m_variables->device.createImageView(imageViewInfo).value);
	}
}

void AlmVulkanRender::CreateRenderpass()
{
	vk::AttachmentDescription colorAttachment;
	colorAttachment.setFormat(m_variables->swapChainImageFormat);
	colorAttachment.setSamples(vk::SampleCountFlagBits::e1);
	colorAttachment.setLoadOp(vk::AttachmentLoadOp::eClear);
	colorAttachment.setStoreOp(vk::AttachmentStoreOp::eStore);
	colorAttachment.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
	colorAttachment.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
	colorAttachment.setInitialLayout(vk::ImageLayout::eUndefined);
	colorAttachment.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

	vk::AttachmentReference attachmentReference;
	attachmentReference.setAttachment(0);
	attachmentReference.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

	vk::SubpassDescription subpass;
	subpass.setColorAttachmentCount(1);
	subpass.setPColorAttachments(&attachmentReference);
	subpass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);

	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.setAttachmentCount(1);
	renderPassInfo.setPAttachments(&colorAttachment);
	renderPassInfo.setSubpassCount(1);
	renderPassInfo.setPSubpasses(&subpass);

	m_variables->renderPass = m_variables->device.createRenderPass(renderPassInfo).value;
}

void AlmVulkanRender::CreateFramebuffers()
{
	m_variables->swapChainFramebuffers.reserve(m_variables->swapChainImageViews.size());
	for (const vk::ImageView &iview : m_variables->swapChainImageViews)
	{
		vk::FramebufferCreateInfo framebufferInfo;
		framebufferInfo.setRenderPass(m_variables->renderPass);
		framebufferInfo.setLayers(1);
		framebufferInfo.setAttachmentCount(1);
		framebufferInfo.setWidth(m_variables->swapChainExtent.width);
		framebufferInfo.setHeight(m_variables->swapChainExtent.height);
		framebufferInfo.setPAttachments(&iview);

		m_variables->swapChainFramebuffers.push_back(m_variables->device.createFramebuffer(framebufferInfo).value);
	}
}

void AlmVulkanRender::CreateCommandPool()
{
	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.setQueueFamilyIndex(m_variables->graphicsFamilyIndex);
	poolInfo.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
	m_variables->commandPool = m_variables->device.createCommandPool(poolInfo).value;

	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.setCommandPool(m_variables->commandPool);
	allocInfo.setLevel(vk::CommandBufferLevel::ePrimary);
	allocInfo.setCommandBufferCount(m_variables->swapChainFramebuffers.size());

	m_variables->commandBuffers = m_variables->device.allocateCommandBuffers(allocInfo).value;
}

void AlmVulkanRender::CreateSynchronization()
{
	for (size_t i(0); i < m_variables->swapChainImages.size(); ++i)
	{
		m_variables->imageAvailableSemaphore.push_back(m_variables->device.createSemaphore(vk::SemaphoreCreateInfo()).value);
		m_variables->renderFinishedSemaphore.push_back(m_variables->device.createSemaphore(vk::SemaphoreCreateInfo()).value);
	}
}



void AlmVulkanRender::DeleteSynchronization()
{
	for (size_t i(0); i < m_variables->swapChainImages.size(); ++i)
	{
		m_variables->device.destroySemaphore(m_variables->renderFinishedSemaphore[i]);
		m_variables->device.destroySemaphore(m_variables->imageAvailableSemaphore[i]);
	}
	m_variables->imageAvailableSemaphore.clear();
	m_variables->renderFinishedSemaphore.clear();
}

void AlmVulkanRender::DeleteCommandPool()
{
	m_variables->device.destroyCommandPool(m_variables->commandPool);
}

void AlmVulkanRender::DeleteFramebuffers()
{
	for (auto framebuffer : m_variables->swapChainFramebuffers)
		m_variables->device.destroyFramebuffer(framebuffer);
	m_variables->swapChainFramebuffers.clear();
}

void AlmVulkanRender::DeleteRenderpass()
{
	m_variables->device.destroyRenderPass(m_variables->renderPass);
}

void AlmVulkanRender::DeleteSwapchain()
{
	for (auto imageView : m_variables->swapChainImageViews)
		m_variables->device.destroyImageView(imageView);
	m_variables->swapChainImageViews.clear();

	m_variables->device.destroySwapchainKHR(m_variables->swapChain);
	m_variables->swapChainImages.clear();
}