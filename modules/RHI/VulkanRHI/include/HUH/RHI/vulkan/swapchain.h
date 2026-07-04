#include "HUH/event.h"

#include <HUH/RHI/swapchain.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <vector>

namespace HUH::RHI {
class VulkanDynamicRHI;
class VulkanDevice;

class HUH_VULKANRHI_API VulkanSwapchain : public Swapchain {
public:
    friend VulkanDevice;
    bool Init(Format format, PresentMode presentMode, Uint32 minImageCount) override;
    void Destroy() override;

    struct SwapchainDetails {
        VkSurfaceCapabilities2KHR capabilities{.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR};
        std::vector<VkSurfaceFormat2KHR> surfaceFormats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    SwapchainDetails Details;

    static VkPresentModeKHR ConvertPresentMode(PresentMode presentMode);

    Image* NextImage(Fence* fence) override;
    Image* NextImage(Fence* fence, Uint64 timeout) override;

    void Present(Queue* queue, Fence* fence) override;

protected:
    bool CreateSwapchain();
    bool RecreateSwapchain();
    explicit VulkanSwapchain(VulkanDevice* device, Window* window, VkSurfaceKHR surface, VulkanDynamicRHI* parent);
    ~VulkanSwapchain() override;

    VkPhysicalDeviceSurfaceInfo2KHR m_surface{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR};
    VkSwapchainKHR m_swapchain = nullptr;
    VulkanDynamicRHI* m_parent;
    VulkanDevice* m_device;
    VkColorSpaceKHR m_colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    VkPresentModeKHR m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
    VkFormat m_vkFormat = VK_FORMAT_UNDEFINED;
    Uint32 m_minImageCount;
    VkExtent2D m_extent = {};
    Uint32 m_imageIndex = 0;
    HUH::EventHandler m_sizeChangeEventHandler;
    HUH::EventHandler m_closeEventHandler;
};
}// namespace HUH::RHI
