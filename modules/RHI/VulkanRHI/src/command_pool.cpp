#include <HUH/Math/vector.h>

#include <HUH/RHI/Types/image.h>
#include <HUH/RHI/vulkan/Types/fence.h>
#include <HUH/RHI/vulkan/Types/image.h>
#include <HUH/RHI/vulkan/command_pool.h>

#include "HUH/RHI/vulkan/Types/buffer.h"

#include <HUH/RHI/vulkan/device.h>
#include <HUH/RHI/vulkan/pipeline.h>
#include <HUH/RHI/vulkan/queue.h>

namespace HUH::RHI {

bool VulkanCommandPool::VulkanCommandBuffer::Begin() {
    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    };

    if (auto err = HUH::vkBeginCommandBuffer(m_commandBuffer, &beginInfo); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "CommandBuffer Begin Error: {}", err)
        return false;
    }

    // TODO REFACTOR BARRIERS TO DIFFERENT CLASS
    VkImageMemoryBarrier2 imageMemoryBarrier{.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
                                             .srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
                                             .srcAccessMask = VK_ACCESS_2_NONE,
                                             .dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
                                             .dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
                                             .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                                             .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                             .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                                             .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                                             .image = m_renderTarget->m_image,
                                             .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                                                  .baseMipLevel = 0,
                                                                  .levelCount = 1,
                                                                  .baseArrayLayer = 0,
                                                                  .layerCount = 1}};
    VkDependencyInfo imageDependencyInfo{
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .dependencyFlags = {},
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &imageMemoryBarrier,
    };
    HUH::vkCmdPipelineBarrier2(m_commandBuffer, &imageDependencyInfo);

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    VkRenderingAttachmentInfo renderingAttachmentInfo = {.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
                                                         .imageView = m_renderTarget->m_imageView,
                                                         .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                                         .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                         .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                                                         .clearValue = clearColor};
    VkRenderingInfo renderingInfo{.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
                                  .renderArea = {.extent = {.width = m_viewPort.X(), .height = m_viewPort.Y()}},
                                  .layerCount = 1,
                                  .colorAttachmentCount = 1,
                                  .pColorAttachments = &renderingAttachmentInfo};

    HUH::vkCmdBeginRendering(m_commandBuffer, &renderingInfo);
    HUH::vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_parent->m_pipeline->m_pipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_viewPort.X());
    viewport.height = static_cast<float>(m_viewPort.Y());
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);

    VkExtent2D extent{m_scissor.X(), m_scissor.Y()};
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = extent;
    vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);

    return true;
}

void VulkanCommandPool::VulkanCommandBuffer::End() {
    HUH::vkCmdEndRendering(m_commandBuffer);
    VkImageMemoryBarrier2 imageMemoryBarrier{.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
                                             .srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
                                             .srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
                                             .dstStageMask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
                                             .dstAccessMask = VK_ACCESS_2_NONE,
                                             .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                             .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                             .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                                             .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                                             .image = m_renderTarget->m_image,
                                             .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                                                  .baseMipLevel = 0,
                                                                  .levelCount = 1,
                                                                  .baseArrayLayer = 0,
                                                                  .layerCount = 1}};
    VkDependencyInfo imageDependencyInfo{
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .dependencyFlags = {},
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &imageMemoryBarrier,
    };
    HUH::vkCmdPipelineBarrier2(m_commandBuffer, &imageDependencyInfo);

    HUH::vkEndCommandBuffer(m_commandBuffer);
}

void VulkanCommandPool::VulkanCommandBuffer::AddRenderTarget(Image* renderTarget) {
    m_renderTarget = dynamic_cast<VulkanImage*>(renderTarget);
}

VulkanCommandPool::VulkanCommandBuffer::~VulkanCommandBuffer() {
    HUH_ILOG(LogVulkanRHI, "CommandBuffer Destruction Successful")
}

bool VulkanCommandPool::Init(Uint32 bufferCount, Queue* queue) {
    m_queue = dynamic_cast<VulkanQueue*>(queue);
    VkCommandPoolCreateInfo poolInfo = {.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                                        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                                        .queueFamilyIndex = static_cast<Uint32>(m_queue->m_familyIndex)};
    if (auto err = HUH::vkCreateCommandPool(*m_device, &poolInfo, nullptr, &m_commandPool); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "CommandBuffer Creation Error: {}", err)
        return false;
    }
    std::vector<VkCommandBuffer> buffersTmp(bufferCount, nullptr);
    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = m_commandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = bufferCount,
    };

    if (auto err = HUH::vkAllocateCommandBuffers(*m_device, &commandBufferAllocateInfo, buffersTmp.data());
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "CommandBuffer Creation Error: {}", err)
        return false;
    }

    m_commandBuffers.reserve(bufferCount);
    for (auto cbuffer : buffersTmp) {
        m_commandBuffers.push_back(new VulkanCommandBuffer(this, cbuffer));
    }

    HUH_ILOG(LogVulkanRHI, "CommandPool Creation Successful")
    return true;
}

void VulkanCommandPool::Destroy() {
    HUH::vkDestroyCommandPool(*m_device, m_commandPool, nullptr);
}

void VulkanCommandPool::VulkanCommandBuffer::Reset() {
    HUH::vkResetCommandBuffer(m_commandBuffer, 0);
}

void VulkanCommandPool::VulkanCommandBuffer::BindBuffer(Buffer* buffer) {
    // TODO offsets
    auto vk_buffer = dynamic_cast<VulkanBuffer*>(buffer);
    VkBuffer buffers[] = {*vk_buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(m_commandBuffer, 0, 1, buffers, offsets);
}

void VulkanCommandPool::VulkanCommandBuffer::Draw(Uint32 vertexCount, Uint32 instanceCount) {
    vkCmdDraw(m_commandBuffer, vertexCount, instanceCount, 0, 0);
}

VulkanCommandPool::VulkanCommandPool(VulkanDevice* device, VulkanPipeline* pipeline)
    : m_device(device),
      m_pipeline(pipeline) {
}

VulkanCommandPool::~VulkanCommandPool() {
    HUH_ILOG(LogVulkanRHI, "VulkanCommandPool Destroyed")
}

}// namespace HUH::RHI