#pragma once
#include "HUH/RHI/Types/fence.h"
#include "HUH/RHI/vulkan/vulkan_defines.h"

namespace HUH::RHI {

template<SyncType>
class HUH_VULKANRHI_API VulkanFence {
    static_assert(false, "Unknown Sync Type");
};

template<>
class HUH_VULKANRHI_API VulkanFence<SyncType::GpuToCpu> : public Fence<SyncType::GpuToCpu> {
public:
    friend class VulkanDevice;

    bool Wait() override;
    bool Wait(Uint64 timeout) override;

    operator VkFence() const { return m_fence; };

protected:
    VulkanFence(class VulkanDevice* device);
    ~VulkanFence() override;

    VulkanDevice* m_device;
    VkFence m_fence = nullptr;
};

template<>
class HUH_VULKANRHI_API VulkanFence<SyncType::GpuToGpu> : public Fence<SyncType::GpuToGpu> {
public:
    friend class VulkanDevice;

    operator VkSemaphore() const { return m_semaphore; };

protected:
    VulkanFence(class VulkanDevice* device);
    ~VulkanFence() override;
    VulkanDevice* m_device;
    VkSemaphore m_semaphore = nullptr;
};

}// namespace HUH::RHI