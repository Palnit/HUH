#pragma once
#include "HUH/RHI/vulkan/memory_allocator.h"

#include <HUH/RHI/Types/barrier.h>

#include <HUH/RHI/vulkan/fwd.h>

namespace HUH::RHI {
class VulkanBarrier : public Barrier {
    friend class VulkanDevice;

public:
    void Init(Initializer&&) override;
    void Destroy() override;

protected:
    void InitImage(Initializer&& initializer);
    void InitBuffer(Initializer&& initializer);
    VulkanBarrier(VulkanDevice* device);
    ~VulkanBarrier() override {}

    VulkanDevice* m_device;
    VulkanBuffer* m_buffer;
    VulkanImage* m_image;
    VkImageMemoryBarrier m_imageMemoryBarrier{.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
    VkBufferMemoryBarrier m_bufferMemoryBarrier{.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER};
};
}// namespace HUH::RHI