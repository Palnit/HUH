#include <HUH/Math/vector.h>
#include <HUH/RHI/vulkan/command_buffer.h>

#include "HUH/RHI/Types/image.h"
#include "HUH/RHI/vulkan/queue.h"
#include "HUH/RHI/vulkan/Types/fence.h"
#include "HUH/RHI/vulkan/Types/image.h"

#include <HUH/RHI/vulkan/device.h>

namespace HUH::RHI {

bool VulkanCommandBuffer::Begin() {
    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };

    if (auto err = HUH::vkBeginCommandBuffer(m_commandBuffer, &beginInfo); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "CommandBuffer Begin Error: {}", err)
        return false;
    }
    // TODO modern version ??

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    VkRenderPassBeginInfo renderPassBeginInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = m_pipeline->m_renderPass,
        .framebuffer = m_frameBuffer,
        .renderArea = m_renderArea,
        .clearValueCount = 1,
        .pClearValues = &clearColor,
    };
    HUH::vkCmdBeginRenderPass(m_commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    HUH::vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->m_pipeline);

    // TODO REFACTOR THIS TO NOT BE TEMPORARY
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_renderArea.extent.width);
    viewport.height = static_cast<float>(m_renderArea.extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = m_renderArea.extent;
    vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);

    vkCmdDraw(m_commandBuffer, 3, 1, 0, 0);

    return true;
}

void VulkanCommandBuffer::End() {
    HUH::vkCmdEndRenderPass(m_commandBuffer);
    if (auto err = HUH::vkEndCommandBuffer(m_commandBuffer); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "CommandBuffer End Error: {}", err)
    }
}

void VulkanCommandBuffer::AddRenderTarget(Image* renderTarget) {
    HUH::vkDestroyFramebuffer(*m_device, m_frameBuffer, nullptr);
    m_frameBuffer = nullptr;
    auto vk_image = dynamic_cast<VulkanImage*>(renderTarget);
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = m_pipeline->m_renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &vk_image->m_imageView;
    framebufferInfo.width = vk_image->m_size.X();
    framebufferInfo.height = vk_image->m_size.Y();
    framebufferInfo.layers = 1;

    if (auto err = HUH::vkCreateFramebuffer(*m_device, &framebufferInfo, nullptr, &m_frameBuffer); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "CommandBuffer Begin Error: {}", err)
    }

    m_renderArea.offset = {0, 0};
    m_renderArea.extent = {vk_image->m_size.X(), vk_image->m_size.Y()};
}

bool VulkanCommandBuffer::Init(Queue* queue) {
    m_queue = dynamic_cast<VulkanQueue*>(queue);
    VkCommandPoolCreateInfo poolInfo = {.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                                        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                                        .queueFamilyIndex = static_cast<Uint32>(m_queue->m_familyIndex)};
    if (auto err = HUH::vkCreateCommandPool(*m_device, &poolInfo, nullptr, &m_commandPool); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "CommandBuffer Creation Error: {}", err)
        return false;
    }
    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = m_commandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };
    if (auto err = HUH::vkAllocateCommandBuffers(*m_device, &commandBufferAllocateInfo, &m_commandBuffer);
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "CommandBuffer Creation Error: {}", err)
        return false;
    }

    HUH_ILOG(LogVulkanRHI, "CommandBuffer Creation Successful")
    return true;
}

void VulkanCommandBuffer::Destroy() {
    HUH::vkDestroyFramebuffer(*m_device, m_frameBuffer, nullptr);
    HUH::vkDestroyCommandPool(*m_device, m_commandPool, nullptr);
}

bool VulkanCommandBuffer::Submit(Fence<SyncType::GpuToGpu>* wait,
                                 Fence<SyncType::GpuToGpu>* signal,
                                 Fence<SyncType::GpuToCpu>* waitSignal) {
    auto vk_wait_fence = dynamic_cast<VulkanFence<SyncType::GpuToGpu>*>(wait);
    auto vk_signal_fence = dynamic_cast<VulkanFence<SyncType::GpuToGpu>*>(signal);
    auto vk_wait_signal_fence = dynamic_cast<VulkanFence<SyncType::GpuToCpu>*>(waitSignal);
    VkSemaphore waitSemaphores[] = {*vk_wait_fence};
    VkPipelineStageFlags waitFlags[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSemaphore signalSemaphores[] = {*vk_signal_fence};
    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = waitSemaphores,
        .pWaitDstStageMask = waitFlags,
        .commandBufferCount = 1,
        .pCommandBuffers = &m_commandBuffer,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = signalSemaphores,
    };

    if (auto err = HUH::vkQueueSubmit(*m_queue, 1, &submitInfo, *vk_wait_signal_fence); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Submit Error: {}", err)
        return false;
    }
    return true;
}// namespace HUH::RHI

void VulkanCommandBuffer::Reset() {
    HUH::vkResetCommandBuffer(m_commandBuffer, 0);
}

VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice* device, VulkanPipeline* pipeline)
    : m_device(device),
      m_pipeline(pipeline) {
}

VulkanCommandBuffer::~VulkanCommandBuffer() {
    HUH_ILOG(LogVulkanRHI, "VulkanCommandBuffer Destroyed")
}

}// namespace HUH::RHI