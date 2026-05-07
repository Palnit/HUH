#pragma once

#include <HUH/RHI/Types/buffer.h>
#include <HUH/RHI/vulkan/fwd.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>
#include "HUH/RHI/vulkan/command_pool.h"
#include "HUH/RHI/vulkan/memory_allocator.h"

namespace HUH::RHI {
class VulkanBuffer : public Buffer {
public:
    friend class VulkanDevice;
    friend class VulkanMemoryAllocator;
    friend class VulkanPipeline;
    friend class VulkanCommandPool::VulkanCommandBuffer;
    static VkBufferUsageFlags ConvertBufferType(Buffer::Type type);
    void Destroy() override;
    void MapData() override;
    void CopyData(void* data) override;
    void UploadData(void* data) override;
    void UnMapData() override;
    HUH_NODISCARD VkMemoryRequirements GetMemoryRequirements() const;
    operator VkBuffer() const { return m_buffer; }
    VulkanMemoryAllocator::MemoryBlock GetMemoryBlock() const { return m_allocatedBlock; }

protected:
    VulkanBuffer(Uint64 size, VkBuffer buffer, VulkanDevice* device);
    ~VulkanBuffer() override = default;

    bool m_uniformBuffer = false;
    Uint32 m_binding = 0;
    VkBuffer m_buffer = nullptr;
    VkMemoryRequirements m_memoryRequirements{};
    VulkanDevice* m_device = nullptr;
    VkDescriptorSet m_descriptorSet = nullptr;
    VulkanMemoryAllocator::MemoryBlock m_allocatedBlock{};
    VulkanMemoryAllocator::Allocation* m_allocation = nullptr;
};
}// namespace HUH::RHI