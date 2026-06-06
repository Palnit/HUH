#include <HUH/Math/vector.h>

#include <HUH/RHI/Types/image.h>
#include <HUH/RHI/vulkan/Types/fence.h>
#include <HUH/RHI/vulkan/Types/image.h>
#include <HUH/RHI/vulkan/command_pool.h>

#include "HUH/RHI/vulkan/Types/barrier.h"
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
                        .y = static_cast<float>(size.Y()),
                        .width = static_cast<float>(size.X()),
                        .height = -static_cast<float>(size.Y()),
                        .minDepth = 0.0f,
                        .maxDepth = 1.0f};
    vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{.offset = {0, 0}, .extent = {m_scissor.X(), m_scissor.Y()}};
    HUH::vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);

    if (m_attachments[0] != *vk_image || m_attachments.size() != 1) {
        vkDestroyFramebuffer(*m_parent->m_device, m_frameBuffer, nullptr);
        m_attachments.clear();
        m_attachments.push_back(*vk_image);
        VkFramebufferCreateInfo framebufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = *vk_renderPass,
            .attachmentCount = static_cast<Uint32>(m_attachments.size()),
            .pAttachments = m_attachments.data(),
            .width = vk_image->GetSize().X(),
            .height = vk_image->GetSize().Y(),
            .layers = 1,
        };
        if (auto err = HUH::vkCreateFramebuffer(*m_parent->m_device, &framebufferCreateInfo, nullptr, &m_frameBuffer);
            err != VK_SUCCESS) {
            HUH_ELOG(LogVulkanRHI, "Error creating vulkan framebuffer: {}", err)
        }
    }

    VkClearValue clearColor = {.color = {{m_clearColor.X(), m_clearColor.Y(), m_clearColor.Z(), m_clearColor.W()}}};
    VkRenderPassBeginInfo renderPassInfo = {.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                                            .renderPass = *vk_renderPass,
                                            .framebuffer = m_frameBuffer,
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

void VulkanCommandPool::VulkanCommandBuffer::BeginRendering(RenderPass* renderPass,
                                                            Image* renderTarget,
                                                            Image* depthTarget) {
    auto vk_image = dynamic_cast<VulkanImage*>(renderTarget);
    auto vk_depth = dynamic_cast<VulkanImage*>(depthTarget);
    auto vk_renderPass = dynamic_cast<VulkanRenderPass*>(renderPass);
    auto size = vk_image->GetSize();

    VkViewport viewport{.x = 0.0f,
                        .y = static_cast<float>(size.Y()),
                        .width = static_cast<float>(size.X()),
                        .height = -static_cast<float>(size.Y()),
                        .minDepth = 0.0f,
                        .maxDepth = 1.0f};
    vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{.offset = {0, 0}, .extent = {m_scissor.X(), m_scissor.Y()}};
    HUH::vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);

    if (m_attachments[0] != *vk_image || m_attachments[1] != *vk_depth || m_attachments.size() != 2) {
        vkDestroyFramebuffer(*m_parent->m_device, m_frameBuffer, nullptr);
        m_attachments.clear();
        m_attachments.push_back(*vk_image);
        m_attachments.push_back(*vk_depth);
        VkFramebufferCreateInfo framebufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = *vk_renderPass,
            .attachmentCount = static_cast<Uint32>(m_attachments.size()),
            .pAttachments = m_attachments.data(),
            .width = vk_image->GetSize().X(),
            .height = vk_image->GetSize().Y(),
            .layers = 1,
        };
        if (auto err = HUH::vkCreateFramebuffer(*m_parent->m_device, &framebufferCreateInfo, nullptr, &m_frameBuffer);
            err != VK_SUCCESS) {
            HUH_ELOG(LogVulkanRHI, "Error creating vulkan framebuffer: {}", err)
        }
    }

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{m_clearColor.X(), m_clearColor.Y(), m_clearColor.Z(), m_clearColor.W()}};
    clearValues[1].depthStencil = {1.0f, 0};
    VkRenderPassBeginInfo renderPassInfo = {.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                                            .renderPass = *vk_renderPass,
                                            .framebuffer = m_frameBuffer,
                                            .renderArea =
                                                {
                                                    .offset = {0, 0},
                                                    // This might need scisor i dont know yet what to do windows
                                                    .extent = {size.X(), size.Y()},
                                                },
                                            .clearValueCount = 2,
                                            .pClearValues = clearValues.data()};
    HUH::vkCmdBeginRenderPass(m_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanCommandPool::VulkanCommandBuffer::EndRendering() {
    vkCmdEndRenderPass(m_commandBuffer);
}

VulkanCommandPool::VulkanCommandBuffer::~VulkanCommandBuffer() {
    HUH_ILOG(LogVulkanRHI, "CommandBuffer Destruction Successful")
    vkDestroyFramebuffer(*m_parent->m_device, m_frameBuffer, nullptr);
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
    CommandPool::Destroy();
    HUH::vkDestroyCommandPool(*m_device, m_commandPool, nullptr);
}

void VulkanCommandPool::VulkanCommandBuffer::Reset() {
    HUH::vkResetCommandBuffer(m_commandBuffer, 0);
}

void VulkanCommandPool::VulkanCommandBuffer::BindVertexBuffer(Buffer* buffer, Uint32 binding) {
    // TODO offsets
    auto vk_buffer = dynamic_cast<VulkanBuffer*>(buffer);
    VkBuffer buffers[] = {*vk_buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(m_commandBuffer, binding, 1, buffers, offsets);
}

void VulkanCommandPool::VulkanCommandBuffer::BindIndexBuffer(Buffer* buffer) {
    auto vk_buffer = dynamic_cast<VulkanBuffer*>(buffer);
    vkCmdBindIndexBuffer(m_commandBuffer, *vk_buffer, 0, VK_INDEX_TYPE_UINT32);
}

void VulkanCommandPool::VulkanCommandBuffer::BindUniformBuffers(Buffer* buffer) {
    if (!m_pipeline) {
        HUH_ELOG(LogVulkanRHI, "You should bind a pipeline before binding buffers")
        return;
    }
    auto vk_buffer = dynamic_cast<VulkanBuffer*>(buffer);
    // TODO RETHINK THIS A LOT
    m_descriptorSets.push_back(vk_buffer->m_descriptorSet);
    m_descriptorWrites.push_back(vk_buffer->m_descriptorWriter);
}

void VulkanCommandPool::VulkanCommandBuffer::BindSampledImage(Image* image) {
    if (!m_pipeline) {
        HUH_ELOG(LogVulkanRHI, "You should bind a pipeline before binding buffers")
        return;
    }
    auto vk_image = dynamic_cast<VulkanImage*>(image);
    m_descriptorSets.push_back(vk_image->m_descriptorSet);
    m_descriptorWrites.push_back(vk_image->GetDescriptorWriter());
}

void VulkanCommandPool::VulkanCommandBuffer::Draw(Uint32 vertexCount, Uint32 instanceCount) {
    BindDescriptorSetWriters();
    vkCmdDraw(m_commandBuffer, vertexCount, instanceCount, 0, 0);
}

void VulkanCommandPool::VulkanCommandBuffer::DrawIndexed(Uint32 indexCount, Uint32 instanceCount) {
    BindDescriptorSetWriters();
    vkCmdDrawIndexed(m_commandBuffer, indexCount, instanceCount, 0, 0, 0);
}

void VulkanCommandPool::VulkanCommandBuffer::BindPipeline(class Pipeline* pipeline) {
    m_pipeline = dynamic_cast<VulkanPipeline*>(pipeline);
    HUH::vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pipeline);
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

void VulkanCommandPool::VulkanCommandBuffer::CopyBuffer(Buffer* srcBuffer, Image* dstImage) {
    auto vk_srcBuffer = dynamic_cast<VulkanBuffer*>(srcBuffer);
    auto vk_dstBuffer = dynamic_cast<VulkanImage*>(dstImage);
    VkBufferImageCopy copyRegion = {
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource =
            {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel = 0,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
        .imageOffset = {0, 0, 0},
        .imageExtent = {vk_dstBuffer->GetSize().Width(), vk_dstBuffer->GetSize().Height(), 1},
    };

    HUH::vkCmdCopyBufferToImage(m_commandBuffer, *vk_srcBuffer, *vk_dstBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
                                &copyRegion);
}

void VulkanCommandPool::VulkanCommandBuffer::BindBarrier(Barrier* barrier) {
    auto vk_barrier = dynamic_cast<VulkanBarrier*>(barrier);
    VkDependencyInfo depInfo = {.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
                                .dependencyFlags = 0,
                                .memoryBarrierCount = 0,
                                .bufferMemoryBarrierCount = 0,
                                .imageMemoryBarrierCount = 1,
                                .pImageMemoryBarriers = &vk_barrier->m_imageMemoryBarrier};

    HUH::vkCmdPipelineBarrier2(m_commandBuffer, &depInfo);
}

void VulkanCommandPool::VulkanCommandBuffer::BindDescriptorSetWriters() {
    vkUpdateDescriptorSets(*m_parent->m_device, m_descriptorWrites.size(), m_descriptorWrites.data(), 0, nullptr);
    for (auto it : m_descriptorSets) {
        vkCmdBindDescriptorSets(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->m_layout, 0, 1, &it, 0,
                                nullptr);
    }
    m_descriptorSets.clear();
    m_descriptorWrites.clear();
}

VulkanCommandPool::VulkanCommandPool(VulkanDevice* device) : m_device(device) {
}

VulkanCommandPool::~VulkanCommandPool() {
    HUH_ILOG(LogVulkanRHI, "VulkanCommandPool Destroyed")
}

}// namespace HUH::RHI