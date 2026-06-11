#pragma once
#include <HUH/RHI/vulkan/fwd.h>
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

#ifdef HUH_USE_CUDA
    Buffer::SharedMemoryInfo GetSharedMemory() override;
#endif

    operator VkFence();
    operator VkSemaphore();

protected:
    void CreateFence(VkFenceCreateFlags flags);
    void CreateSemaphore();

    Buffer::SharedMemoryInfo m_handle{};

    VulkanFence(VulkanDevice* device);
    ~VulkanFence() override;

    VulkanDevice* m_device;
    VkFence m_fence = nullptr;
    VkSemaphore m_semaphore = nullptr;
};

}// namespace HUH::RHI