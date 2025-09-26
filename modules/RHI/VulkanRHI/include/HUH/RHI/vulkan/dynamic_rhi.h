#pragma once

#include <HUH/RHI/vulkan/vulkan_loader.h>
#include <HUH/RHI/dynamic_rhi.h>

namespace HUH::RHI {
class HUH_API VulkanDynamicRHI final : public RHI::DynamicRHI {
public:
    void Destroy() override;
    bool Init() override;

protected:
    VkInstance m_instance = nullptr;
};
}// namespace HUH::RHI
