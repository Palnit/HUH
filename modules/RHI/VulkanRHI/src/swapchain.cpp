#include "HUH/RHI/vulkan/device.h"

#include <HUH/RHI/vulkan/swapchain.h>
#include <HUH/RHI/vulkan/dynamic_rhi.h>
namespace HUH::RHI {

bool VulkanSwapchain::Init(Device* device) {
    const VulkanDevice* vk_device = static_cast<VulkanDevice*>(device);
    if (!vk_device) {
        return false;
    }
    HUH::vkGetPhysicalDeviceSurfaceCapabilities2KHR(vk_device->m_physicalDevice, &m_surface, &Details.capabilities);
    HUH_ILOG(LogVulkanRHI, "Min image count {}", Details.capabilities.surfaceCapabilities.minImageCount)
    HUH_ILOG(LogVulkanRHI, "Max image count {}", Details.capabilities.surfaceCapabilities.maxImageCount)
}

VulkanSwapchain::VulkanSwapchain(VkSurfaceKHR surface, VulkanDynamicRHI* parent) : Swapchain(), m_parent(parent) {
    m_surface.surface = surface;
}

VulkanSwapchain::~VulkanSwapchain() {
    HUH::vkDestroySurfaceKHR(m_parent->m_instance, m_surface.surface, nullptr);
}
}// namespace HUH::RHI