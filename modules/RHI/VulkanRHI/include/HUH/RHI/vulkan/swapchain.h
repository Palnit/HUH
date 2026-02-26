#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/RHI/swapchain.h>

namespace HUH::RHI {
class VulkanDynamicRHI;
class HUH_VULKANRHI_API VulkanSwapchain : public Swapchain {
public:
    friend class VulkanDynamicRHI;
    bool Init(Device* device) override;

    struct SwapchainDetails {
        VkSurfaceCapabilities2KHR capabilities{.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR};
        std::vector<VkSurfaceFormat2KHR> surfaceFormats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    SwapchainDetails Details;

private:
    VulkanSwapchain(VkSurfaceKHR surface, VulkanDynamicRHI* parent);
    ~VulkanSwapchain() override;

    VkPhysicalDeviceSurfaceInfo2KHR m_surface{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR};
    VulkanDynamicRHI* m_parent;
};
}// namespace HUH::RHI
