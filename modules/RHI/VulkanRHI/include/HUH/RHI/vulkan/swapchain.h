#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/RHI/swapchain.h>

namespace HUH::RHI {
class VulkanDynamicRHI;
class HUH_VULKANRHI_API VulkanSwapchain : public Swapchain {
public:
    friend class VulkanDynamicRHI;

private:
    VulkanSwapchain(VkSurfaceKHR surface, VulkanDynamicRHI* parent)
        : Swapchain(),
          m_surface(surface),
          m_parent(parent) {};
    ~VulkanSwapchain() override;
    VkSurfaceKHR m_surface;
    VulkanDynamicRHI* m_parent;
};
}// namespace HUH::RHI
