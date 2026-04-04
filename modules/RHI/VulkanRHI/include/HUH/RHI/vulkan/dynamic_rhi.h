#pragma once

#include <HUH/RHI/dynamic_rhi.h>
#include <HUH/RHI/vulkan/fwd.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>

namespace HUH::RHI {

extern "C" HUH_VULKANRHI_API HUH::RHI::DynamicRHI* DynamicRHICreate();

class HUH_VULKANRHI_API VulkanDynamicRHI final : public RHI::DynamicRHI {
public:
    friend class VulkanSwapchain;
    friend class VulkanDevice;
    void Destroy() override;
    bool Init() override;
    Device* GetDevice(size_t index) override;
    std::vector<Device*> GetDevices() override;

    HUH_VULKANRHI_API friend HUH::RHI::DynamicRHI* DynamicRHICreate();

    static VkFormat ConvertFormat(Format format);
    static Format ConvertFormat(VkFormat format);

protected:
    VulkanDynamicRHI() = default;
    ~VulkanDynamicRHI() override;
    VkInstance m_instance = nullptr;
};
}// namespace HUH::RHI
