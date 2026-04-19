#include <HUH/RHI/vulkan/Types/buffer.h>
#include <HUH/RHI/vulkan/device.h>

namespace HUH::RHI {
VkBufferUsageFlags VulkanBuffer::ConvertBufferType(Buffer::Type type) {
    VkBufferUsageFlags result = 0;
    if (HUH::CheckFlag(type, HUH::RHI::Buffer::VERTEX)) {
        result |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    }
    if (HUH::CheckFlag(type, HUH::RHI::Buffer::INDEX)) {
        result |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    }
    if (HUH::CheckFlag(type, HUH::RHI::Buffer::UNIFORM)) {
        result |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    }
    if (HUH::CheckFlag(type, HUH::RHI::Buffer::SRC)) {
        result |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    }
    if (HUH::CheckFlag(type, HUH::RHI::Buffer::DST)) {
        result |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }
    return result;
}

void VulkanBuffer::Destroy() {
    if (m_buffer) {
        HUH::vkDestroyBuffer(*m_device, m_buffer, nullptr);
    }
}

void VulkanBuffer::CopyData(void* data) {
    void* tmp;
    if (auto err =
            HUH::vkMapMemory(*m_device, m_allocation->Memory, m_allocatedBlock.Offset, m_allocatedBlock.Size, 0, &tmp);
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Error while mapping vulkan memory Error: {}", err)
        return;
    }
    std::memcpy(tmp, data, m_size);
    // TODO flush
    HUH::vkUnmapMemory(*m_device, m_allocation->Memory);
}

VkMemoryRequirements VulkanBuffer::GetMemoryRequirements() const {
    return m_memoryRequirements;
}
VulkanBuffer::VulkanBuffer(Uint64 size, VkBuffer buffer, VulkanDevice* device)
    : Buffer(size),
      m_buffer(buffer),
      m_device(device) {
    HUH::vkGetBufferMemoryRequirements(*m_device, m_buffer, &m_memoryRequirements);
}
}// namespace HUH::RHI