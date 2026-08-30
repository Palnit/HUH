#include <HUH/RHI/vulkan/Types/fence.h>
#include <HUH/RHI/vulkan/device.h>
#include <HUH/RHI/vulkan/queue.h>

#include "HUH/RHI/vulkan/command_pool.h"
#include "HUH/RHI/vulkan/pipeline.h"

namespace HUH::RHI {
VulkanQueue::operator VkQueue() {
    if (m_queue) {
        return m_queue;
    }
    HUH::vkGetDeviceQueue(*m_device, m_familyIndex, m_queueIndex, &m_queue);
    return m_queue;
}

bool VulkanQueue::Submit(CommandPool::CommandBuffer* commandPool, Fence* wait, Fence* signal, Fence* waitSignal) {
    auto vk_CommandBuffer = dynamic_cast<VulkanCommandPool::VulkanCommandBuffer*>(commandPool);
    auto vk_wait_fence = dynamic_cast<VulkanFence*>(wait);
    auto vk_signal_fence = dynamic_cast<VulkanFence*>(signal);
    auto vk_wait_signal_fence = dynamic_cast<VulkanFence*>(waitSignal);
    VkSemaphore waitSemaphores[] = {*vk_wait_fence};
    VkPipelineStageFlags waitFlags[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSemaphore signalSemaphores[] = {*vk_signal_fence};
    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = waitSemaphores,
        .pWaitDstStageMask = waitFlags,
        .commandBufferCount = 1,
        .pCommandBuffers = &vk_CommandBuffer->m_commandBuffer,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = signalSemaphores,
    };

    if (auto err = HUH::vkQueueSubmit(*this, 1, &submitInfo, *vk_wait_signal_fence); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Submit Error: {}", err)
        return false;
    }
    return true;
}

bool VulkanQueue::Submit(CommandPool::CommandBuffer* commandPool,
                         HUH::Array<WaitFence> wait,
                         HUH::Array<Fence*> signal,
                         Fence* waitSignal) {

    auto vk_CommandBuffer = dynamic_cast<VulkanCommandPool::VulkanCommandBuffer*>(commandPool);
    auto vk_wait_signal_fence = dynamic_cast<VulkanFence*>(waitSignal);
    HUH::Array<VkSemaphore> waitSemaphores;
    HUH::Array<VkPipelineStageFlags> waitStages;
    for (auto& waitFence : wait) {
        auto vk_wait_fence = dynamic_cast<VulkanFence*>(waitFence.waitFence);
        waitSemaphores.Emplace(*vk_wait_fence);
        waitStages.Emplace(HUH::RHI::VulkanPipeline::ConvertToPipelineStage(waitFence.stage));
    }

    HUH::Array<VkSemaphore> signalSemaphores;
    for (auto& signalFence : signal) {
        auto vk_signal_fence = dynamic_cast<VulkanFence*>(signalFence);
        signalSemaphores.Emplace(*vk_signal_fence);
    }

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = static_cast<HUH ::Uint32>(waitSemaphores.size()),
        .pWaitSemaphores = waitSemaphores.empty() ? nullptr : waitSemaphores.data(),
        .pWaitDstStageMask = waitSemaphores.empty() ? nullptr : waitStages.data(),
        .commandBufferCount = 1,
        .pCommandBuffers = &vk_CommandBuffer->m_commandBuffer,
        .signalSemaphoreCount = static_cast<HUH ::Uint32>(signalSemaphores.size()),
        .pSignalSemaphores = signalSemaphores.empty() ? nullptr : signalSemaphores.data(),
    };

    if (auto err = HUH::vkQueueSubmit(*this, 1, &submitInfo, *vk_wait_signal_fence); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Submit Error: {}", err)
        return false;
    }
    return true;
}

bool VulkanQueue::Submit(CommandPool::CommandBuffer* commandPool) {

    auto vk_CommandBuffer = dynamic_cast<VulkanCommandPool::VulkanCommandBuffer*>(commandPool);
    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &vk_CommandBuffer->m_commandBuffer,
    };

    if (auto err = HUH::vkQueueSubmit(*this, 1, &submitInfo, nullptr); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Submit Error: {}", err)
        return false;
    }
    return true;
}

void VulkanQueue::WaitIdle() {
    HUH::vkQueueWaitIdle(*this);
}

}// namespace HUH::RHI