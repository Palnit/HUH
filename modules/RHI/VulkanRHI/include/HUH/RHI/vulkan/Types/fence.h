#pragma once
#include "HUH/RHI/Types/fence.h"
#include "HUH/RHI/vulkan/vulkan_defines.h"

namespace HUH::RHI {

class HUH_VULKANRHI_API VulkanFence : public Fence {
public:
    friend class VulkanDevice;

    bool Wait() override;
    bool Wait(Uint64 timeout) override;
    void Reset() override;

    operator VkFence() const { return m_fence; }
    operator VkSemaphore() const { return m_semaphore; }

    operator VkFence();
    operator VkSemaphore();

protected:
    void CreateFence(VkFenceCreateFlags flags);
    void CreateSemaphore();

    VulkanFence(class VulkanDevice* device);
    ~VulkanFence() override;

    VulkanDevice* m_device;
    VkFence m_fence = nullptr;
    VkSemaphore m_semaphore = nullptr;
};

}// namespace HUH::RHI