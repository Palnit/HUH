#include <HUH/Math/vector.h>
#include <HUH/RHI/vulkan/command_buffer.h>

#include "HUH/RHI/Types/image.h"
#include "HUH/RHI/vulkan/queue.h"
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
    auto vk_queue = dynamic_cast<VulkanQueue*>(queue);
    VkCommandPoolCreateInfo poolInfo = {.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                                        .queueFamilyIndex = static_cast<Uint32>(vk_queue->m_familyIndex)};
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
    HUH::vkDestroyCommandPool(*m_device, m_commandPool, nullptr);
}

bool VulkanCommandBuffer::Submit() {
    return false;
}

void VulkanCommandBuffer::Reset() {
}

VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice* device, VulkanPipeline* pipeline)
    : m_device(device),
      m_pipeline(pipeline) {
}

VulkanCommandBuffer::~VulkanCommandBuffer() {
    HUH_ILOG(LogVulkanRHI, "VulkanCommandBuffer Destroyed")
}

}// namespace HUH::RHI