#include "alm_vulkanrender.hpp"
#include "../alm_log.hpp"
#include "../alm_platform.hpp"

#include <string>
#include <vector>
#include <array>
#include <vulkan/vulkan.hpp>

using namespace alme;

static vk::Device globVKDevice;
static vk::Instance globVKInstance;
static vk::SurfaceKHR globVKSurface;
static vk::SwapchainKHR globVKSwapchain;
static vk::PhysicalDevice globVKPhysicalDevice;

static vk::Queue globVKQueue;
static float globVKQueuePriority;
static uint32_t globVKQueueFamilyIndex;

static uint32_t globVKCurrentBuffer;
static vk::CommandPool globVKCommandPool;
static std::vector<vk::CommandBuffer> globVKCommandBuffers;

static vk::Rect2D globVKRenderArea;
static vk::Viewport globVKViewport;
static vk::Extent2D globVKSurfaceSize;

static vk::Format globVKSurfaceColorFormat;
static vk::Format globVKSurfaceDepthFormat;
static vk::ColorSpaceKHR globVKSurfaceColorSpace;

// Swpachain
struct SwapChainBuffer 
{
	vk::Image image;
	std::array<vk::ImageView, 2> views;
	vk::Framebuffer frameBuffer;
};

static std::vector<SwapChainBuffer> globVKSwapchainBuffers;

// Sync
static std::vector<vk::Fence> globVKWaitFences;
static vk::Semaphore globVKRenderCompleteSemaphore;
static vk::Semaphore globVKPresentCompleteSemaphore;

static void findBestExtensions(const std::vector<vk::ExtensionProperties>& installed, const std::vector<const char*>& wanted, std::vector<const char*>& out)
{
	for (const char* const& w : wanted) {
		for (vk::ExtensionProperties const& i : installed) {
			if (std::string(i.extensionName).compare(w) == 0) {
				out.emplace_back(w);
				break;
			}
		}
	}
}

static void findBestLayers(const std::vector<vk::LayerProperties>& installed, const std::vector<const char*>& wanted, std::vector<const char*>& out)
{
	for (const char* const& w : wanted) {
		for (vk::LayerProperties const& i : installed) {
			if (std::string(i.layerName).compare(w) == 0) {
				out.emplace_back(w);
				break;
			}
		}
	}
}



AlmVulkanRender::AlmVulkanRender()
{
}

AlmVulkanRender::~AlmVulkanRender()
{
	globVKDevice.destroyCommandPool(globVKCommandPool);
	globVKDevice.destroy();
	globVKInstance.destroy();
}


void AlmVulkanRender::InitRenderAPIInstance()
{
	InitInstance();
	InitSurface();
	InitDevicePhys();
	InitDeviceLogic();
	SetupSwapchain(640, 480);
	InitCommandPool();
	InitSynch();
}

void AlmVulkanRender::OnWindowResize(unsigned int width, unsigned int height)
{
	globVKDevice.waitIdle();
	DestroyCommands();
	CreateCommands();
	SetupCommands();
	SetupSwapchain(width, height);
}

void AlmVulkanRender::InitInstance()
{
	std::vector<vk::ExtensionProperties> installedExtensions = vk::enumerateInstanceExtensionProperties();
	std::vector<const char*> wantedExtensions =
	{
		VK_KHR_SURFACE_EXTENSION_NAME,
#if defined(VK_USE_PLATFORM_WIN32_KHR)
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
		VK_MVK_MACOS_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XCB_KHR)
		VK_KHR_XCB_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
		VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
		VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XCB_KHR)
		VK_KHR_XCB_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
		VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_MIR_KHR) || defined(VK_USE_PLATFORM_DISPLAY_KHR)
		VK_KHR_DISPLAY_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
		VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_IOS_MVK)
		VK_MVK_IOS_SURFACE_EXTENSION_NAME
#endif
	};

	std::vector<const char*> extNames = {};
	findBestExtensions(installedExtensions, wantedExtensions, extNames);
	std::vector<const char*>wantedLayers =
	{
#ifdef _DEBUG
		"VK_LAYER_LUNARG_standard_validation"
#endif
	};

	std::vector<const char*> layerNames = {};
	std::vector<vk::LayerProperties> installedLayers = vk::enumerateInstanceLayerProperties();
	findBestLayers(installedLayers, wantedLayers, layerNames);

	vk::ApplicationInfo app_info("sandbox", 1, "AlmostEngine", 1, VK_API_VERSION_1_1);
	vk::InstanceCreateInfo info(vk::InstanceCreateFlags(), &app_info, layerNames.size(), layerNames.data(), extNames.size(), extNames.data());

	vk::Result res = vk::createInstance(&info, nullptr, &globVKInstance);
	ALM_LOG_ASSERT(res == vk::Result::eSuccess, "Failed to init vulkan instance!");
}

void AlmVulkanRender::InitDevicePhys()
{
	bool bassert = false;
	for (auto & device : globVKInstance.enumeratePhysicalDevices())
	{
		globVKQueueFamilyIndex = 0;
		for (auto queueFamily : device.getQueueFamilyProperties())
		{
			if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				bassert = true;
				globVKPhysicalDevice = device;
				break;
			}
			++globVKQueueFamilyIndex;
		}
	}

	ALM_LOG_ASSERT(bassert, "Failed to find physical device!");

	std::vector<vk::SurfaceFormatKHR> surfaceFormats = globVKPhysicalDevice.getSurfaceFormatsKHR(globVKSurface);
	if (surfaceFormats.size() == 1 && surfaceFormats[0].format == vk::Format::eUndefined)
		globVKSurfaceColorFormat = vk::Format::eB8G8R8A8Unorm;
	else
		globVKSurfaceColorFormat = surfaceFormats[0].format;
	globVKSurfaceColorSpace = surfaceFormats[0].colorSpace;


	std::vector<vk::Format> depthFormats = {
		vk::Format::eD32SfloatS8Uint,
		vk::Format::eD32Sfloat,
		vk::Format::eD24UnormS8Uint,
		vk::Format::eD16UnormS8Uint,
		vk::Format::eD16Unorm
	};

	for (vk::Format& format : depthFormats)
	{
		vk::FormatProperties depthFormatProperties = globVKPhysicalDevice.getFormatProperties(format);
		if (depthFormatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
		{
			globVKSurfaceDepthFormat = format;
			break;
		}
	}
}

void AlmVulkanRender::InitDeviceLogic()
{
	std::vector<vk::ExtensionProperties> installedDeviceExtensions = globVKPhysicalDevice.enumerateDeviceExtensionProperties();
	std::vector<const char*> deviceExtensions = {};
	std::vector<const char*> wantedDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	findBestExtensions(installedDeviceExtensions, wantedDeviceExtensions, deviceExtensions);

	vk::DeviceQueueCreateInfo qcinfo;
	qcinfo.setQueueFamilyIndex(globVKQueueFamilyIndex);
	qcinfo.setQueueCount(1);
	globVKQueuePriority = 0.5f;
	qcinfo.setPQueuePriorities(&globVKQueuePriority);

	vk::DeviceCreateInfo dinfo;
	dinfo.setPQueueCreateInfos(&qcinfo);
	dinfo.setQueueCreateInfoCount(1);
	dinfo.setPpEnabledExtensionNames(deviceExtensions.data());
	dinfo.setEnabledExtensionCount(static_cast<uint32_t>(deviceExtensions.size()));

	globVKDevice = globVKPhysicalDevice.createDevice(dinfo);
	globVKQueue = globVKDevice.getQueue(globVKQueueFamilyIndex, 0);
}

void AlmVulkanRender::InitCommandPool()
{
	vk::CommandPoolCreateInfo info(
		vk::CommandPoolCreateFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer),
		globVKQueueFamilyIndex
	);
	globVKCommandPool = globVKDevice.createCommandPool(info);
}

void alme::AlmVulkanRender::InitSurface()
{
#ifdef ALM_OS_WINDOWS

	HWND hwnd = GetActiveWindow();
	vk::Win32SurfaceCreateInfoKHR info(vk::Win32SurfaceCreateFlagsKHR(), GetModuleHandle(nullptr), hwnd);
	globVKSurface = globVKInstance.createWin32SurfaceKHR(info);

#else
	ALM_LOG_ASSERT(false, "Not implemented render surface!");
#endif // ALM_OS_WINDOWS
}

void AlmVulkanRender::InitSynch()
{
	// Semaphore used to ensures that image presentation is complete before starting to submit again
	globVKPresentCompleteSemaphore = globVKDevice.createSemaphore(vk::SemaphoreCreateInfo());

	// Semaphore used to ensures that all commands submitted have been finished before submitting the image to the queue
	globVKRenderCompleteSemaphore = globVKDevice.createSemaphore(vk::SemaphoreCreateInfo());

	// Fence for command buffer completion
	globVKWaitFences.resize(globVKSwapchainBuffers.size());

	for (size_t i = 0; i < globVKWaitFences.size(); i++)
	{
		globVKWaitFences[i] = globVKDevice.createFence(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));
	}
}




void alme::AlmVulkanRender::DestroyCommands()
{
}

void alme::AlmVulkanRender::CreateCommands()
{
	vk::CommandBufferAllocateInfo info(
		globVKCommandPool,
		vk::CommandBufferLevel::ePrimary,
		static_cast<uint32_t>(globVKSwapchainBuffers.size())
	);
	globVKCommandBuffers = globVKDevice.allocateCommandBuffers(info);
}

void alme::AlmVulkanRender::SetupCommands()
{
}

void AlmVulkanRender::SetupSwapchain(unsigned int width, unsigned int height)
{
	// Setup viewports, Vsync
	vk::Extent2D swapchainSize = vk::Extent2D(width, height);

	// All framebuffers / attachments will be the same size as the surface
	vk::SurfaceCapabilitiesKHR surfaceCapabilities = globVKPhysicalDevice.getSurfaceCapabilitiesKHR(globVKSurface);
	if (!(surfaceCapabilities.currentExtent.width == -1 || surfaceCapabilities.currentExtent.height == -1)) {
		swapchainSize = surfaceCapabilities.currentExtent;
		globVKRenderArea = vk::Rect2D(vk::Offset2D(), swapchainSize);
		globVKViewport = vk::Viewport(0.0f, 0.0f, static_cast<float>(swapchainSize.width), static_cast<float>(swapchainSize.height), 0, 1.0f);
	}

	// VSync
	std::vector<vk::PresentModeKHR> surfacePresentModes = globVKPhysicalDevice.getSurfacePresentModesKHR(globVKSurface);
	vk::PresentModeKHR presentMode = vk::PresentModeKHR::eImmediate;

	for (vk::PresentModeKHR& pm : surfacePresentModes) {
		if (pm == vk::PresentModeKHR::eMailbox) {
			presentMode = vk::PresentModeKHR::eMailbox;
			break;
		}
	}

	// Create Swapchain, Images, Frame Buffers

	globVKDevice.waitIdle();
	vk::SwapchainKHR oldSwapchain = globVKSwapchain;

	// Some devices can support more than 2 buffers, but during my tests they would crash on fullscreen ~ ag
	// Tested on an NVIDIA 1080 and 165 Hz 2K display
	auto clamp = [](uint32_t a, uint32_t mi, uint32_t mx) -> uint32_t { return a < mi ? mi : (a > mx ? mx : a); };
	uint32_t backbufferCount = clamp(surfaceCapabilities.maxImageCount, 1U, 2U);

	globVKSwapchain = globVKDevice.createSwapchainKHR(
		vk::SwapchainCreateInfoKHR(
			vk::SwapchainCreateFlagsKHR(),
			globVKSurface,
			backbufferCount,
			globVKSurfaceColorFormat,
			globVKSurfaceColorSpace,
			swapchainSize,
			1,
			vk::ImageUsageFlagBits::eColorAttachment,
			vk::SharingMode::eExclusive,
			1,
			&globVKQueueFamilyIndex,
			vk::SurfaceTransformFlagBitsKHR::eIdentity,
			vk::CompositeAlphaFlagBitsKHR::eOpaque,
			presentMode,
			VK_TRUE,
			oldSwapchain
		)
	);

	globVKSurfaceSize = vk::Extent2D(clamp(swapchainSize.width, 1U, 8192U), clamp(swapchainSize.height, 1U, 8192U));
	globVKRenderArea = vk::Rect2D(vk::Offset2D(), globVKSurfaceSize);
	globVKViewport = vk::Viewport(0.0f, 0.0f, static_cast<float>(globVKSurfaceSize.width), static_cast<float>(globVKSurfaceSize.height), 0, 1.0f);


	// Destroy previous swapchain
	if (oldSwapchain != vk::SwapchainKHR(nullptr))
	{
		globVKDevice.destroySwapchainKHR(oldSwapchain);
	}

	// Resize swapchain buffers for use later
	globVKSwapchainBuffers.resize(backbufferCount);
}