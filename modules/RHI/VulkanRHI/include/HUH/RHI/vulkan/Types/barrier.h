#pragma once
#include "HUH/RHI/vulkan/command_pool.h"
#include "HUH/RHI/vulkan/memory_allocator.h"

#include <HUH/RHI/Types/barrier.h>

#include <HUH/RHI/vulkan/fwd.h>

namespace HUH::RHI {
class VulkanBarrier : public Barrier {
    friend class VulkanDevice;
    friend class VulkanCommandPool::VulkanCommandBuffer;

public:
    void Init(Initializer&&) override;
    void Destroy() override;

protected:
    void InitImage(Initializer&& initializer);
    void InitBuffer(Initializer&& initializer);
    explicit VulkanBarrier(VulkanDevice* device);
    ~VulkanBarrier() override = default;

    VulkanDevice* m_device;
    VulkanBuffer* m_buffer;
    VulkanImage* m_image;
    VkImageMemoryBarrier2 m_imageMemoryBarrier{.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2};
    VkBufferMemoryBarrier2 m_bufferMemoryBarrier{.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2};
};
}// namespace HUH::RHI