#include <HUH/RHI/vulkan/Types/barrier.h>

#include "HUH/RHI/vulkan/Types/buffer.h"
#include "HUH/RHI/vulkan/Types/image.h"
#include "HUH/RHI/vulkan/dynamic_rhi.h"
#include "HUH/RHI/vulkan/pipeline.h"
#include "HUH/RHI/vulkan/render_pass.h"

namespace HUH::RHI {

void VulkanBarrier::Init(Initializer&& initializer) {
    if (initializer.Buffer) {
        m_buffer = dynamic_cast<VulkanBuffer*>(initializer.Buffer);
        InitBuffer(std::move(initializer));
    }
    if (initializer.Image) {
        m_image = dynamic_cast<VulkanImage*>(initializer.Image);
        InitImage(std::move(initializer));
    }
}

void VulkanBarrier::Destroy() {
    delete this;
}

void VulkanBarrier::InitImage(Initializer&& initializer) {
    m_imageMemoryBarrier.srcStageMask = VulkanPipeline::ConvertToPipelineStage(initializer.Transitions.Image.srcStage);
    m_imageMemoryBarrier.dstStageMask = VulkanPipeline::ConvertToPipelineStage(initializer.Transitions.Image.dstStage);
    m_imageMemoryBarrier.srcAccessMask =
        HUH::RHI::VulkanDynamicRHI::ConvertAccess(initializer.Transitions.Image.srcAccess);
    m_imageMemoryBarrier.dstAccessMask =
        HUH::RHI::VulkanDynamicRHI::ConvertAccess(initializer.Transitions.Image.dstAccess);
    m_imageMemoryBarrier.oldLayout =
        HUH::RHI::VulkanRenderPass::ConvertImageLayout(initializer.Transitions.Image.srcLayout);
    m_imageMemoryBarrier.newLayout =
        HUH::RHI::VulkanRenderPass::ConvertImageLayout(initializer.Transitions.Image.dstLayout);
    m_imageMemoryBarrier.srcQueueFamilyIndex = (initializer.Transitions.Image.srcQueue == -1)
        ? VK_QUEUE_FAMILY_IGNORED
        : static_cast<Uint32>(initializer.Transitions.Image.srcQueue);
    m_imageMemoryBarrier.dstQueueFamilyIndex = (initializer.Transitions.Image.dstQueue == -1)
        ? VK_QUEUE_FAMILY_IGNORED
        : static_cast<Uint32>(initializer.Transitions.Image.dstQueue);

    // TODO depth??
    m_imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    m_imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
    m_imageMemoryBarrier.subresourceRange.levelCount = 1;
    m_imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
    m_imageMemoryBarrier.subresourceRange.layerCount = 1;

    m_imageMemoryBarrier.image = *m_image;
}

void VulkanBarrier::InitBuffer(Initializer&& initializer) {
}

VulkanBarrier::VulkanBarrier(VulkanDevice* device) : Barrier(), m_device(device) {
}
}// namespace HUH::RHI