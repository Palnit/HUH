#include <HUH/RHI/vulkan/Types/fence.h>
#include <HUH/RHI/vulkan/device.h>

namespace HUH::RHI {

bool VulkanFence<SyncType::GpuToCpu>::Wait() {
    return Wait(std::numeric_limits<Uint64>::max());
}

bool VulkanFence<SyncType::GpuToCpu>::Wait(const Uint64 timeout) {
    if (HUH::vkWaitForFences(*m_device, 1, &m_fence, VK_TRUE, timeout) != VK_SUCCESS) {
        HUH::vkResetFences(*m_device, 1, &m_fence);
        return false;
    }
    HUH::vkResetFences(*m_device, 1, &m_fence);
    return true;
}

VulkanFence<HUH::RHI::SyncType::GpuToCpu>::VulkanFence(class VulkanDevice* device) : Fence(), m_device(device) {
    VkFenceCreateInfo fenceCreateInfo{.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                                      .flags = VK_FENCE_CREATE_SIGNALED_BIT};
    if (auto err = HUH::vkCreateFence(*m_device, &fenceCreateInfo, nullptr, &m_fence); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Vulkan Fence Creation Error: {}", err)
    }
    HUH_ILOG(LogVulkanRHI, "Vulkan Fence Creation Successful")
}

VulkanFence<SyncType::GpuToCpu>::~VulkanFence() {
    HUH::vkDestroyFence(*m_device, m_fence, nullptr);
    HUH_ILOG(LogVulkanRHI, "Fence<GpuToCpu> Destroyed")
}

}// namespace HUH::RHI

namespace HUH::RHI {

VulkanFence<SyncType::GpuToGpu>::VulkanFence(class VulkanDevice* device) : Fence(), m_device(device) {
    VkSemaphoreCreateInfo semaphoreCreateInfo{.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    if (auto err = HUH::vkCreateSemaphore(*m_device, &semaphoreCreateInfo, nullptr, &m_semaphore); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Vulkan Semaphore Creation Error: {}", err)
    }
    HUH_ILOG(LogVulkanRHI, "Vulkan Semaphore Creation Successful")
}

VulkanFence<SyncType::GpuToGpu>::~VulkanFence() {
    HUH::vkDestroySemaphore(*m_device, m_semaphore, nullptr);
    HUH_ILOG(LogVulkanRHI, "Fence<GpuToGpu> Destroyed")
}
}// namespace HUH::RHI