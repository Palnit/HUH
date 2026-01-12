#pragma once

#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/RHI/dynamic_rhi.h>

namespace HUH::RHI {
class VulkanSwapchain;

extern "C" HUH_VULKANRHI_API HUH::RHI::DynamicRHI* DynamicRHICreate();

class HUH_VULKANRHI_API VulkanDynamicRHI final : public RHI::DynamicRHI {
public:
    friend class VulkanSwapchain;
    void Destroy() override;
    bool Init() override;
    Device* GetDevice(size_t index) override;
    std::vector<Device*> GetDevices() override;
    Swapchain* CreateSurface(const Window& window) override;

    friend HUH::RHI::DynamicRHI* DynamicRHICreate();

protected:
    VulkanDynamicRHI() = default;
    ~VulkanDynamicRHI() override = default;
    VkInstance m_instance = nullptr;
};
}// namespace HUH::RHI
