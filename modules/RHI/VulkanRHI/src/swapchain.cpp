#include <HUH/RHI/vulkan/swapchain.h>
#include <HUH/RHI/vulkan/dynamic_rhi.h>
namespace HUH::RHI {
VulkanSwapchain::~VulkanSwapchain() {
    HUH::vkDestroySurfaceKHR(m_parent->m_instance, m_surface, nullptr);
}
}// namespace HUH::RHI