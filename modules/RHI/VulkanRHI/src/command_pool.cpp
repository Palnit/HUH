#include <HUH/Math/vector.h>

#include <HUH/RHI/Types/image.h>
#include <HUH/RHI/vulkan/Types/fence.h>
#include <HUH/RHI/vulkan/Types/image.h>
#include <HUH/RHI/vulkan/command_pool.h>

#include "HUH/RHI/vulkan/Types/buffer.h"
#include "HUH/RHI/vulkan/render_pass.h"

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
    return true;
}

void VulkanCommandPool::VulkanCommandBuffer::End() {
    HUH::vkEndCommandBuffer(m_commandBuffer);
}

void VulkanCommandPool::VulkanCommandBuffer::BeginRendering(RenderPass* renderPass, Image* renderTarget) {
    auto vk_image = dynamic_cast<VulkanImage*>(renderTarget);
    auto vk_renderPass = dynamic_cast<VulkanRenderPass*>(renderPass);
    auto size = vk_image->GetSize();

    VkViewport viewport{.x = 0.0f,
                        .y = 0.0f,
                        .width = static_cast<float>(size.X()),
                        .height = static_cast<float>(size.Y()),
                        .minDepth = 0.0f,
                        .maxDepth = 1.0f};
    vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{.offset = {0, 0}, .extent = {m_scissor.X(), m_scissor.Y()}};
    HUH::vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);

    VkClearValue clearColor = {.color = {{m_clearColor.X(), m_clearColor.Y(), m_clearColor.Z(), m_clearColor.W()}}};
    VkRenderPassBeginInfo renderPassInfo = {.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                                            .renderPass = *vk_renderPass,
                                            .framebuffer = vk_image->GetFrameBuffer(vk_renderPass),
                                            .renderArea =
                                                {
                                                    .offset = {0, 0},
                                                    // This might need scisor i dont know yet what to do windows
                                                    .extent = {size.X(), size.Y()},
                                                },
                                            .clearValueCount = 1,
                                            .pClearValues = &clearColor};
    HUH::vkCmdBeginRenderPass(m_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanCommandPool::VulkanCommandBuffer::EndRendering() {
    vkCmdEndRenderPass(m_commandBuffer);
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

void VulkanCommandPool::VulkanCommandBuffer::BindVertexBuffer(Buffer* buffer) {
    // TODO offsets
    auto vk_buffer = dynamic_cast<VulkanBuffer*>(buffer);
    VkBuffer buffers[] = {*vk_buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(m_commandBuffer, 0, 1, buffers, offsets);
}

void VulkanCommandPool::VulkanCommandBuffer::BindIndexBuffer(Buffer* buffer) {
    auto vk_buffer = dynamic_cast<VulkanBuffer*>(buffer);
    vkCmdBindIndexBuffer(m_commandBuffer, *vk_buffer, 0, VK_INDEX_TYPE_UINT32);
}

void VulkanCommandPool::VulkanCommandBuffer::Draw(Uint32 vertexCount, Uint32 instanceCount) {
    vkCmdDraw(m_commandBuffer, vertexCount, instanceCount, 0, 0);
}

void VulkanCommandPool::VulkanCommandBuffer::DrawIndexed(Uint32 indexCount, Uint32 instanceCount) {
    vkCmdDrawIndexed(m_commandBuffer, indexCount, instanceCount, 0, 0, 0);
}

void VulkanCommandPool::VulkanCommandBuffer::BindPipeline(class Pipeline* pipeline) {
    auto vk_pipeline = dynamic_cast<VulkanPipeline*>(pipeline);
    HUH::vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *vk_pipeline);
}

void VulkanCommandPool::VulkanCommandBuffer::CopyBuffer(Buffer* srcBuffer, Buffer* dstBuffer) {
    auto vk_srcBuffer = dynamic_cast<VulkanBuffer*>(srcBuffer);
    auto vk_dstBuffer = dynamic_cast<VulkanBuffer*>(dstBuffer);
    if (vk_srcBuffer->GetSize() != vk_dstBuffer->GetSize()) {
        HUH_WLOG(LogVulkanRHI, "The size of the two buffer to copy are not equal: src: {} dst: {}",
                 vk_srcBuffer->GetMemoryBlock().Size, vk_dstBuffer->GetMemoryBlock().Size)
    }
    VkBufferCopy copyRegion = {.srcOffset = 0, .dstOffset = 0, .size = vk_srcBuffer->GetSize()};
    vkCmdCopyBuffer(m_commandBuffer, *vk_srcBuffer, *vk_dstBuffer, 1, &copyRegion);
}

VulkanCommandPool::VulkanCommandPool(VulkanDevice* device) : m_device(device) {
}

VulkanCommandPool::~VulkanCommandPool() {
    HUH_ILOG(LogVulkanRHI, "VulkanCommandPool Destroyed")
}

}// namespace HUH::RHI