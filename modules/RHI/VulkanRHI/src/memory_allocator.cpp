#include <HUH/RHI/vulkan/Types/buffer.h>
#include <HUH/RHI/vulkan/memory_allocator.h>

#include "HUH/RHI/vulkan/device.h"

namespace HUH::RHI {

void VulkanMemoryAllocator::Destroy() {
}

bool VulkanMemoryAllocator::Allocate(Buffer* buffer, Type type) {
    auto vk_buffer = dynamic_cast<VulkanBuffer*>(buffer);
    auto requirments = vk_buffer->GetMemoryRequirements();
}

bool VulkanMemoryAllocator::Free(Buffer* buffer) {
}

VkMemoryPropertyFlags VulkanMemoryAllocator::ConvertMemoryType(MemoryAllocator::Type type) {
    VkMemoryPropertyFlags flags = 0;
    if (HUH::CheckFlag(type, RHI::MemoryAllocator::Type::Device)) {
        flags |= VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }
    if (HUH::CheckFlag(type, RHI::MemoryAllocator::Type::Host)) {
        flags |= VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    }
    return flags;
}

Uint64 VulkanMemoryAllocator::FindMemoryType(VkMemoryRequirements requirements, VkMemoryPropertyFlags properties) {
}

VulkanMemoryAllocator::MemoryBlock VulkanMemoryAllocator::Allocation::Allocate(Uint64 size, Uint64 alignment) {
}

bool VulkanMemoryAllocator::Allocation::Free(MemoryBlock block) {
    FreeBlocks.push_back(block);
    return true;
}

VulkanMemoryAllocator::VulkanMemoryAllocator(VulkanDevice* device) : m_device(device) {
}

VulkanMemoryAllocator::~VulkanMemoryAllocator() {
}

}// namespace HUH::RHI
