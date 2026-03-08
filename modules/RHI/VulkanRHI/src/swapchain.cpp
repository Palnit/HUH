#include <HUH/RHI/vulkan/device.h>
#include <HUH/RHI/vulkan/swapchain.h>
#include <HUH/Window/window.h>
#include <HUH/RHI/vulkan/Types/image.h>
#include <HUH/RHI/vulkan/dynamic_rhi.h>

namespace HUH::RHI {

bool VulkanSwapchain::Init(Format format, PresentMode presentMode, Uint32 minImageCount) {
    if (!m_device) {
        HUH_ELOG(LogVulkanRHI, "None vulkan device give to vulkan swapchain")
        return false;
    }
    HUH::vkGetPhysicalDeviceSurfaceCapabilities2KHR(m_device->m_physicalDevice, &m_surface, &Details.capabilities);
    Uint32 size = 0;
    HUH::vkGetPhysicalDeviceSurfaceFormats2KHR(m_device->m_physicalDevice, &m_surface, &size, nullptr);
    Details.surfaceFormats.resize(size, {.sType = VK_STRUCTURE_TYPE_SURFACE_FORMAT_2_KHR});
    HUH::vkGetPhysicalDeviceSurfaceFormats2KHR(m_device->m_physicalDevice, &m_surface, &size,
                                               Details.surfaceFormats.data());
#ifdef HUH_DEBUG
    HUH_LOG(LogVulkanRHI, Logging::DebugLog, "Available Swapchain surface formates and colorspace:")
    for (size_t i = 0; i < Details.surfaceFormats.size(); i++) {
        HUH_LOG(LogVulkanRHI, Logging::DebugLog, "{}.\tFormate: {}, Color Space: {}", i + 1,
                Details.surfaceFormats[i].surfaceFormat.format, Details.surfaceFormats[i].surfaceFormat.colorSpace)
    }
#endif

    HUH::vkGetPhysicalDeviceSurfacePresentModesKHR(m_device->m_physicalDevice, m_surface.surface, &size, nullptr);
    Details.presentModes.resize(size);
    HUH::vkGetPhysicalDeviceSurfacePresentModesKHR(m_device->m_physicalDevice, m_surface.surface, &size,
                                                   Details.presentModes.data());

#ifdef HUH_DEBUG
    HUH_LOG(LogVulkanRHI, Logging::DebugLog, "Available present modes:")
    for (size_t i = 0; i < Details.presentModes.size(); i++) {
        HUH_LOG(LogVulkanRHI, Logging::DebugLog, "{}.\t{}", i + 1, Details.presentModes[i])
    }
#endif

    const auto vk_requestedFormat = VulkanDynamicRHI::ConvertFormat(format);
    VkColorSpaceKHR colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    bool foundFormat = false;
    for (auto surfaceFormat : Details.surfaceFormats) {
        if (surfaceFormat.surfaceFormat.format == vk_requestedFormat) {
            colorSpace = surfaceFormat.surfaceFormat.colorSpace;
            foundFormat = true;
        }
    }

    if (!foundFormat) {
        // TODO LOG FORMAT AS STRING
        HUH_ELOG(LogVulkanRHI, "Invalid format requested of swapchain");
        return false;
    }

    if (minImageCount < Details.capabilities.surfaceCapabilities.minImageCount
        || minImageCount > Details.capabilities.surfaceCapabilities.maxImageCount) {
        HUH_ELOG(LogVulkanRHI, "Invalid Image Count Requested: {}, Min Image Count: {}, Max Image Count {}",
                 minImageCount, Details.capabilities.surfaceCapabilities.minImageCount,
                 Details.capabilities.surfaceCapabilities.maxImageCount);
        return false;
    }
    VkExtent2D swapchainExtent = Details.capabilities.surfaceCapabilities.currentExtent;

    if (Details.capabilities.surfaceCapabilities.currentExtent.width == 0xFFFFFFFF) {
        swapchainExtent.height = m_windowParent->GetHeight();
        swapchainExtent.width = m_windowParent->GetWidth();
    }

    VkPresentModeKHR vk_requestedPresentMode = ConvertPresentMode(presentMode);
    VkPresentModeKHR foundPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (auto vk_presentMode : Details.presentModes) {
        if (vk_presentMode == vk_requestedPresentMode) {
            foundPresentMode = vk_presentMode;
        }
    }

    VkSwapchainCreateInfoKHR createInfo = {.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                                           .surface = m_surface.surface,
                                           .minImageCount = minImageCount,
                                           .imageFormat = vk_requestedFormat,
                                           .imageColorSpace = colorSpace,
                                           .imageExtent = swapchainExtent,
                                           .imageArrayLayers = 1,
                                           .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                           .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
                                           .queueFamilyIndexCount = 0,
                                           .pQueueFamilyIndices = nullptr,
                                           .preTransform = Details.capabilities.surfaceCapabilities.currentTransform,
                                           .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                                           .presentMode = foundPresentMode,
                                           .clipped = true,
                                           .oldSwapchain = VK_NULL_HANDLE};
    if (auto err = HUH::vkCreateSwapchainKHR(m_device->m_device, &createInfo, nullptr, &m_swapchain);
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Swapchain Creation Error: {}", err)
        return false;
    }

    Uint32 imageCount = 0;
    std::vector<VkImage> images;
    HUH::vkGetSwapchainImagesKHR(m_device->m_device, m_swapchain, &imageCount, nullptr);
    images.resize(imageCount);
    HUH::vkGetSwapchainImagesKHR(m_device->m_device, m_swapchain, &imageCount, images.data());
    for (auto image : images) {
        m_images.push_back(new VulkanImage(image));
        m_images.back()->Init({m_device, format, 1, {m_extent.width, m_extent.height}});
    }

    HUH_ILOG(LogVulkanRHI, "Swapchain Creation Successful")

    return true;
}

void VulkanSwapchain::Destroy() {
    Swapchain::Destroy();
    HUH::vkDestroySwapchainKHR(m_device->m_device, m_swapchain, nullptr);
    HUH::vkDestroySurfaceKHR(m_parent->m_instance, m_surface.surface, nullptr);
}

VkPresentModeKHR VulkanSwapchain::ConvertPresentMode(PresentMode presentMode) {
    switch (presentMode) {
        case PresentMode::Immediate:
            return VK_PRESENT_MODE_FIFO_KHR;
        case PresentMode::VSync:
            return VK_PRESENT_MODE_IMMEDIATE_KHR;
        default:
            return VK_PRESENT_MODE_FIFO_KHR;
    }
}

VulkanSwapchain::VulkanSwapchain(VulkanDevice* device, Window* window, VkSurfaceKHR surface, VulkanDynamicRHI* parent)
    : Swapchain(window),
      m_parent(parent),
      m_device(device) {
    m_surface.surface = surface;
}

VulkanSwapchain::~VulkanSwapchain() {
    HUH_ILOG(LogVulkanRHI, "Vulkan Swapchain destroyed");
}
}// namespace HUH::RHI