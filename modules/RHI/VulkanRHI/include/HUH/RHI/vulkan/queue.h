#pragma once

#include <HUH/RHI/queue.h>
#include <HUH/RHI/vulkan/fwd.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>

namespace HUH::RHI {

class HUH_VULKANRHI_API VulkanQueue final : public Queue {
    friend class VulkanDevice;
    friend class VulkanCommandPool;
    friend class VulkanSwapchain;

public:
    operator VkQueue();
    bool Submit(CommandPool::CommandBuffer* commandPool, Fence* wait, Fence* signal, Fence* waitSignal) override;
    bool Submit(CommandPool::CommandBuffer* commandPool,
                std::vector<WaitFence> wait,
                std::vector<Fence*> signal,
                Fence* waitSignal) override;
    bool Submit(CommandPool::CommandBuffer* commandPool) override;
    void WaitIdle() override;

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
    }

    VulkanDevice* m_device;
    size_t m_familyIndex = 0;
    size_t m_queueIndex = 0;
    VkQueueFamilyProperties m_properties;
    VkQueue m_queue = nullptr;
};
}// namespace HUH::RHI
