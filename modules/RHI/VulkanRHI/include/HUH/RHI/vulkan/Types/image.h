#pragma once

#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/RHI/Types/image.h>

namespace HUH::RHI {
class HUH_VULKANRHI_API VulkanImage : public Image {
public:
    friend class VulkanDynamicRHI;
    friend class VulkanSwapchain;
    friend class VulkanCommandPool;

    bool Init(Initializer&& init) override;
    void Destroy() override;

    operator VkImageView() const { return m_imageView; }

protected:
    explicit VulkanImage(VkImage image) : Image(false), m_image(image) {}
    ~VulkanImage() override;

    VkImage m_image;
    VkImageView m_imageView = nullptr;
    class VulkanDevice* m_device = nullptr;
};
}// namespace HUH::RHI