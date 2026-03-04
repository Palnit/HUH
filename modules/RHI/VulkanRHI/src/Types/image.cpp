#include <HUH/RHI/vulkan/Types/image.h>

#include "HUH/RHI/vulkan/device.h"
#include "HUH/RHI/vulkan/dynamic_rhi.h"
namespace HUH::RHI {

void VulkanImage::Destroy() {
    if (!m_device) {
        return;
    }
    if (m_created) {
    }
    HUH::vkDestroyImageView(m_device->m_device, m_imageView, nullptr);
}

bool VulkanImage::Init(Intializer&& init) {
    // TODO proper vulkan image creation from real image or swapchain image
    m_device = dynamic_cast<VulkanDevice*>(init.Device);
    VkComponentMapping componentMapping = {
        .r = VK_COMPONENT_SWIZZLE_IDENTITY,
        .g = VK_COMPONENT_SWIZZLE_IDENTITY,
        .b = VK_COMPONENT_SWIZZLE_IDENTITY,
        .a = VK_COMPONENT_SWIZZLE_IDENTITY,
    };
    VkImageSubresourceRange imageSubresourceRange = {

        // TODO proper aspect mask
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = init.MipLevels,
        .baseArrayLayer = 0,
        .layerCount = 1,
    };
    VkImageViewCreateInfo imageCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = m_image,
        // TODO proper image view type of 2d or 3d
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VulkanDynamicRHI::ConvertFormat(init.Format),
        .components = componentMapping,
        .subresourceRange = imageSubresourceRange,
    };

    if (auto err = HUH::vkCreateImageView(m_device->m_device, &imageCreateInfo, nullptr, &m_imageView);
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Image Creation Error: {}", err)
        return false;
    }
    HUH_ILOG(LogVulkanRHI, "Image Creation Success");

    return true;
}

VulkanImage::~VulkanImage() {
    HUH_ILOG(LogVulkanRHI, "Image Destroyed");
}

}// namespace HUH::RHI