#pragma once

#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/RHI/queue.h>

namespace HUH::RHI {

class VulkanQueue final : public Queue {
    friend class VulkanDevice;

public:
private:
    VulkanQueue(const size_t familyIndex, const size_t queueIndex, const VkQueueFamilyProperties& properties)
        : Queue(),
          m_familyIndex(familyIndex),
          m_queueIndex(queueIndex),
          m_properties(properties) {}
    ~VulkanQueue() override {
        HUH_ILOG(LogVulkanRHI, "Destroying VulkanQueue: Family {}, Queue {}", m_familyIndex, m_queueIndex);
    };
    size_t m_familyIndex = 0;
    size_t m_queueIndex = 0;
    VkQueueFamilyProperties m_properties;
};
}// namespace HUH::RHI
