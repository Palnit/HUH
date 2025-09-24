#pragma once

#include <HUH/RHI/dynamic_rhi.h>
#include <HUH/RHI/vulkan/vulkan_loader.h>

namespace HUH::RHI {
class HUH_API VulkanDynamicRHI final : public RHI::DynamicRHI {
public:
    void Destroy() override;
    bool Init() override;

protected:
    VkInstance m_instance = nullptr;
};
}// namespace HUH::RHI
