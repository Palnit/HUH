#pragma once

#include "HUH/RHI/vulkan/memory_allocator.h"

#include <HUH/RHI/Types/buffer.h>
#include <HUH/RHI/vulkan/fwd.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>

namespace HUH::RHI {
class VulkanBuffer : public Buffer {
public:
    friend class VulkanDevice;
    friend class VulkanMemoryAllocator;
    static VkBufferUsageFlags ConvertBufferType(Buffer::Type type);
    void Destroy() override;
    HUH_NODISCARD VkMemoryRequirements GetMemoryRequirements() const;

protected:
    VulkanBuffer(Uint64 size, VkBuffer buffer, VulkanDevice* device);
    ~VulkanBuffer() override = default;

    VkBuffer m_buffer = nullptr;
    VkMemoryRequirements m_memoryRequirements{};
    VulkanDevice* m_device = nullptr;
    VulkanMemoryAllocator::MemoryBlock AllocatedBlock{};
};
}// namespace HUH::RHI