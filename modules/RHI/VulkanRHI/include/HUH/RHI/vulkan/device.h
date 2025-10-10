#pragma once
#include "vulkan_defines.h"
#include "HUH/logging.h"

#include <HUH/definitions.h>
#include <HUH/RHI/device.h>
#include <HUH/vulkan_loader.h>

namespace HUH::RHI {
class HUH_API VulkanDevice final : public Device {
    friend class VulkanDynamicRHI;

public:
    Device::Type GetType() override { return m_type; }
    Device::MemoryStatistics GetMemoryStatistics() override;
    bool Init(DeviceInitializer&& initialization) override { return true; }
    void Destroy() override {}

protected:
    explicit VulkanDevice(VkPhysicalDevice physicalDevice);
    ~VulkanDevice() override{HUH_LOG(LogVulkanRHI, Logging::Level::Log, "Destroyed")}

    VkPhysicalDevice m_physicalDevice = nullptr;
    VkDevice m_device = nullptr;
};

}// namespace HUH::RHI
