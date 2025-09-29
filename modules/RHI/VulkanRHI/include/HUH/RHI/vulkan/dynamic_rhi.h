#pragma once

#include <HUH/vulkan_loader.h>
#include <HUH/RHI/dynamic_rhi.h>

namespace HUH::RHI {

extern "C" HUH_API HUH::RHI::DynamicRHI* DynamicRHICreate();

class HUH_API VulkanDynamicRHI final : public RHI::DynamicRHI {
public:
    void Destroy() override;
    bool Init() override;
    Device* GetDevice(size_t index) override;
    std::vector<Device*> GetDevices() override;

    friend HUH::RHI::DynamicRHI* DynamicRHICreate();

protected:
    VulkanDynamicRHI() = default;
    ~VulkanDynamicRHI() override = default;

    VkInstance m_instance = nullptr;
};
}// namespace HUH::RHI
