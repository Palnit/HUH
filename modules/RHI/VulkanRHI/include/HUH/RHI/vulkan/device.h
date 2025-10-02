#pragma once
#include <HUH/definitions.h>
#include <HUH/RHI/device.h>
#include <HUH/vulkan_loader.h>

namespace HUH::RHI {
class HUH_API VulkanDevice final : public Device {
public:
    HUH_CONSTEXPR_FORCE Device::Type GetType() override { return Type::Dedicated; }

protected:
    VulkanDevice() = default;
    ~VulkanDevice() override = default;
    VkPhysicalDevice m_physicalDevice = nullptr;
    VkDevice m_device = nullptr;
};

}// namespace HUH::RHI
