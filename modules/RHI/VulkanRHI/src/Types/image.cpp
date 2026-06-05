#include <HUH/RHI/vulkan/Types/image.h>

#include "HUH/enum_helper.h"

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
    HUH::vkDestroySampler(*m_device, m_sampler, nullptr);
}

VkSampler VulkanImage::GetSampler() {
    // TODO make these not exclusive to vulkan image?
    if (m_sampler) {
        return m_sampler;
    }
    VkSamplerCreateInfo samplerCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = VK_FILTER_LINEAR,
        .minFilter = VK_FILTER_LINEAR,
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
        .addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
        .addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
        .mipLodBias = 0,
        .anisotropyEnable = VK_TRUE,
        .maxAnisotropy = m_device->Properties.properties_1_0.properties.limits.maxSamplerAnisotropy,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .minLod = 0.0,
        .maxLod = 0.0,
        .borderColor = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK,
        .unnormalizedCoordinates = VK_FALSE};

    if (auto err = vkCreateSampler(*m_device, &samplerCreateInfo, nullptr, &m_sampler); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Error While creating Sampler: {}", err)
        return nullptr;
    }
    return m_sampler;
}

VkWriteDescriptorSet VulkanImage::GetDescriptorWriter() {

    m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    m_imageInfo.imageView = m_imageView;
    m_imageInfo.sampler = GetSampler();

    m_descriptorWriter = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = m_descriptorSet,
        .dstBinding = m_binding,
        .dstArrayElement = 0,
        .descriptorCount = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .pImageInfo = &m_imageInfo,
        .pBufferInfo = nullptr,
        .pTexelBufferView = nullptr,
    };
    return m_descriptorWriter;
}

VkMemoryRequirements VulkanImage::GetMemoryRequirements() const {
    return m_memoryRequirements;
}

VkImageUsageFlags VulkanImage::ConvertImageUsage(Image::Type type) {
    VkImageUsageFlags result = 0;
    if (HUH::CheckFlag(type, HUH::RHI::Image::Color)) {
        result |= VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
    if (HUH::CheckFlag(type, HUH::RHI::Image::DeptStencil)) {
        result |= VkImageUsageFlagBits::VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }
    if (HUH::CheckFlag(type, HUH::RHI::Image::Transient)) {
        result |= VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
    }
    if (HUH::CheckFlag(type, HUH::RHI::Image::Storage)) {
        result |= VkImageUsageFlagBits::VK_IMAGE_USAGE_STORAGE_BIT;
    }
    if (HUH::CheckFlag(type, HUH::RHI::Image::Sampled)) {
        result |= VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT;
    }
    if (HUH::CheckFlag(type, HUH::RHI::Image::SRC)) {
        result |= VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }
    if (HUH::CheckFlag(type, HUH::RHI::Image::DST)) {
        result |= VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }
    return result;
}

VulkanImage::VulkanImage(VulkanDevice* device, VkImage image, bool created)
    : Image(created),
      m_image(image),
      m_device(device) {
    vkGetImageMemoryRequirements(*m_device, m_image, &m_memoryRequirements);
}

bool VulkanImage::Init(Initializer&& init) {
    m_size = init.Size;

    VkImageAspectFlags aspects;
    if (init.Format == Format::D32_FLOAT) {
        aspects = VK_IMAGE_ASPECT_DEPTH_BIT;
    } else if (init.Format == Format::D32_FLOAT_S8_UINT) {
        aspects = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

    } else if (init.Format == Format::D24_UNORM_S8_UINT) {
        aspects = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    } else {
        aspects = VK_IMAGE_ASPECT_COLOR_BIT;
    }

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
                .aspectMask = aspects,
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