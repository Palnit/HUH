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

protected:
    explicit VulkanDevice(VkPhysicalDevice physicalDevice) : m_physicalDevice(physicalDevice) {
        VkPhysicalDeviceProperties properties;
        HUH::vkGetPhysicalDeviceProperties(m_physicalDevice, &properties);
        switch (properties.deviceType) {
            case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                m_type = Type::Other;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                m_type = Type::Software;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                m_type = Type::Cpu;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                m_type = Type::Integrated;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                m_type = Type::Dedicated;
                break;
            default:;
        }
    }
    ~VulkanDevice() override { HUH_LOG(LogVulkanRHI, Logging::Level::Log, "Destroyed") };

    VkPhysicalDevice m_physicalDevice = nullptr;
    VkDevice m_device = nullptr;
    Device::Type m_type = Device::Type::Unknown;
};

}// namespace HUH::RHI
