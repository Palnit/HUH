#pragma once

#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/RHI/queue.h>

namespace HUH::RHI {

class HUH_VULKANRHI_API VulkanQueue final : public Queue {
    friend class VulkanDevice;

public:
private:
    VulkanQueue(class VulkanDevice* device,
                const size_t familyIndex,
                const size_t queueIndex,
                const VkQueueFamilyProperties& properties)
        : Queue(),
          m_device(device),
          m_familyIndex(familyIndex),
          m_queueIndex(queueIndex),
          m_properties(properties) {}
    ~VulkanQueue() override {
        HUH_ILOG(LogVulkanRHI, "Destroying VulkanQueue: Family {}, Queue {}", m_familyIndex, m_queueIndex);
    };
    VulkanDevice* m_device;
    size_t m_familyIndex = 0;
    size_t m_queueIndex = 0;
    VkQueueFamilyProperties m_properties;
};
}// namespace HUH::RHI
