#include <HUH/RHI/vulkan/Types/image.h>
#include <HUH/RHI/vulkan/device.h>
#include <HUH/RHI/vulkan/dynamic_rhi.h>
#include <HUH/RHI/vulkan/pipeline.h>
#include <HUH/RHI/vulkan/render_pass.h>

namespace HUH::RHI {

void VulkanImage::Destroy() {
    if (!m_device) {
        return;
    }
    if (m_created) {
        HUH::vkDestroyImage(*m_device, m_image, nullptr);
    }
    HUH::vkDestroyImageView(*m_device, m_imageView, nullptr);
    HUH::vkDestroyFramebuffer(*m_device, m_frameBuffer, nullptr);
}

VkFramebuffer VulkanImage::GetFrameBuffer(const VulkanRenderPass* renderPass) {
    if (m_frameBuffer) {
        return m_frameBuffer;
    }
    VkImageView imageViews[] = {m_imageView};
    VkFramebufferCreateInfo framebufferCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = *renderPass,
        .attachmentCount = 1,
        .pAttachments = imageViews,
        .width = m_size.X(),
        .height = m_size.Y(),
        .layers = 1,
    };
    if (auto err = HUH::vkCreateFramebuffer(*m_device, &framebufferCreateInfo, nullptr, &m_frameBuffer);
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Error creating vulkan framebuffer: {}", err)
    }
    return m_frameBuffer;
}

VkMemoryRequirements VulkanImage::GetMemoryRequirements() const {
    return m_memoryRequirements;
}

bool VulkanImage::Init(Initializer&& init) {
    m_device = dynamic_cast<VulkanDevice*>(init.Device);
    m_size = init.Size;
    if (m_created) {
        vkGetImageMemoryRequirements(*m_device, m_image, &m_memoryRequirements);
    }
    // TODO proper vulkan image creation from real image or swapchain image

    VkImageViewCreateInfo imageCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = m_image,
        // TODO proper image view type of 2d or 3d
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VulkanDynamicRHI::ConvertFormat(init.Format),
        .components =
            {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY,
            },
        .subresourceRange =
            {

                // TODO proper aspect mask
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = init.MipLevels,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
    };

    if (auto err = HUH::vkCreateImageView(m_device->m_device, &imageCreateInfo, nullptr, &m_imageView);
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Image Creation Error: {}", err)
        return false;
    }
    HUH_ILOG(LogVulkanRHI, "Image Creation Successful");

    return true;
}

VulkanImage::~VulkanImage() {
    HUH_ILOG(LogVulkanRHI, "VulkanImage Destroyed");
}

}// namespace HUH::RHI