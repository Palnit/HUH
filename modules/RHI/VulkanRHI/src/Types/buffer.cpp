#include <HUH/RHI/vulkan/Types/buffer.h>

#include <HUH/RHI/vulkan/device.h>

#include <HUH/enum_helper.h>

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
    // UnMapData();
    if (m_buffer) {
        HUH::vkDestroyBuffer(*m_device, m_buffer, nullptr);
    }
}
void VulkanBuffer::MapData() {
    if (m_mappedData) {
        return;
    }
    if (auto err = HUH::vkMapMemory(*m_device, m_allocation->Memory, m_allocatedBlock.Offset, m_allocatedBlock.Size, 0,
                                    &m_mappedData);
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Error while mapping vulkan memory Error: {}", err)
    }
}
void VulkanBuffer::UnMapData() {
    // TODO flush
    if (m_mappedData) {
        HUH::vkUnmapMemory(*m_device, m_allocation->Memory);
        m_mappedData = nullptr;
    }
}

void VulkanBuffer::CopyData(void* data) {
    MapData();
    std::memcpy(m_mappedData, data, m_size);
    UnMapData();
}

void VulkanBuffer::UploadData(void* data) {
    MapData();
    std::memcpy(m_mappedData, data, m_size);
    if (m_uniformBuffer) {

        // TODO other than than uniform buffer;
        m_bufferInfo = {
            .buffer = m_buffer,
            .offset = 0,
            .range = m_size,
        };
        m_descriptorWriter = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = m_descriptorSet,
            .dstBinding = m_binding,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pImageInfo = nullptr,// Optional
            .pBufferInfo = &m_bufferInfo,
            .pTexelBufferView = nullptr,// Optional
        };

        // TODO whole set update
    }
    UnMapData();
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