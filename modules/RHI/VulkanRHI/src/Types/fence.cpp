#include <HUH/RHI/vulkan/Types/fence.h>
#include <HUH/RHI/vulkan/device.h>
#undef max

namespace HUH::RHI {

bool VulkanFence::Wait() {

    return Wait(std::numeric_limits<Uint64>::max());
}

bool VulkanFence::Wait(const Uint64 timeout) {
    CreateFence(VK_FENCE_CREATE_SIGNALED_BIT);
    if (HUH::vkWaitForFences(*m_device, 1, &m_fence, VK_TRUE, timeout) != VK_SUCCESS) {
        HUH::vkResetFences(*m_device, 1, &m_fence);
        return false;
    }
    return true;
}

void VulkanFence::Reset() {
    if (m_fence) {
        HUH::vkResetFences(*m_device, 1, &m_fence);
    }
}

VulkanFence::operator VkFence() {
    CreateFence(0);
    return m_fence;
}

VulkanFence::operator VkSemaphore() {
    CreateSemaphore();
    return m_semaphore;
}

void VulkanFence::CreateFence(VkFenceCreateFlags flags) {
    if (m_fence) {
        return;
    }
    VkFenceCreateInfo fenceCreateInfo{.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .flags = flags};
    if (auto err = HUH::vkCreateFence(*m_device, &fenceCreateInfo, nullptr, &m_fence); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Vulkan Fence Creation Error: {}", err)
    }
    HUH_ILOG(LogVulkanRHI, "Fence Creation Successful")
}

void VulkanFence::CreateSemaphore() {
    if (m_semaphore) {
        return;
    }
    VkSemaphoreCreateInfo semaphoreCreateInfo{.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    if (auto err = HUH::vkCreateSemaphore(*m_device, &semaphoreCreateInfo, nullptr, &m_semaphore); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Vulkan Semaphore Creation Error: {}", err)
    }
    HUH_ILOG(LogVulkanRHI, "Vulkan Semaphore Creation Successful")
}

VulkanFence::VulkanFence(class VulkanDevice* device) : Fence(), m_device(device) {
}

VulkanFence::~VulkanFence() {
    if (m_fence) {
        HUH::vkDestroyFence(*m_device, m_fence, nullptr);
    }
    if (m_semaphore) {
        HUH::vkDestroySemaphore(*m_device, m_semaphore, nullptr);
    }
    HUH_ILOG(LogVulkanRHI, "Fence Destroyed")
}

}// namespace HUH::RHI