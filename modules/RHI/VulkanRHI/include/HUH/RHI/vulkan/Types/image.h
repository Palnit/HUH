#pragma once

#include <HUH/RHI/Types/image.h>

#include <HUH/RHI/vulkan/command_pool.h>
#include <HUH/RHI/vulkan/fwd.h>
#include <HUH/RHI/vulkan/memory_allocator.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>

namespace HUH::RHI {
class HUH_VULKANRHI_API VulkanImage : public Image {
public:
    friend class VulkanDynamicRHI;
    friend class VulkanSwapchain;
    friend class VulkanCommandPool;
    friend class VulkanDevice;
    friend class VulkanMemoryAllocator;
    friend class VulkanPipeline;
    friend class VulkanCommandPool::VulkanCommandBuffer;

    bool Init(Initializer&& init) override;
    void Destroy() override;

    operator VkImageView() const { return m_imageView; }
    // TODO change location
    VkFramebuffer GetFrameBuffer(const VulkanRenderPass* renderPass);

    operator VkImage() const { return m_image; }

    VkMemoryRequirements GetMemoryRequirements() const;

protected:
    explicit VulkanImage(VkImage image, bool created = false) : Image(created), m_image(image) {}
    ~VulkanImage() override;

    Uint32 m_binding = 0;
    VkImage m_image = nullptr;
    VkImageView m_imageView = nullptr;
    VkFramebuffer m_frameBuffer = nullptr;
    VulkanDevice* m_device = nullptr;
    VkDescriptorSet m_descriptorSet = nullptr;
    VkWriteDescriptorSet m_descriptorWriter = {};
    VkDescriptorBufferInfo m_bufferInfo = {};
    VkMemoryRequirements m_memoryRequirements{};
    VulkanMemoryAllocator::MemoryBlock m_allocatedBlock{};
    VulkanMemoryAllocator::Allocation* m_allocation = nullptr;
};
}// namespace HUH::RHI