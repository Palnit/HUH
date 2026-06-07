#include <HUH/RHI/vulkan/memory_allocator.h>

#include "HUH/RHI/vulkan/Types/image.h"

#include <HUH/RHI/vulkan/Types/buffer.h>
#include <HUH/RHI/vulkan/device.h>

#include <HUH/enum_helper.h>

namespace HUH::RHI {

void VulkanMemoryAllocator::Destroy() {
}

bool VulkanMemoryAllocator::Allocate(Buffer* buffer, Type type) {
    const auto vk_buffer = dynamic_cast<VulkanBuffer*>(buffer);
    const auto requirements = vk_buffer->GetMemoryRequirements();
    const auto typeIndex = FindMemoryType(requirements, ConvertMemoryType(type));
    // TODO customizable base size
    const Uint32 size = requirements.size > 64000000 ? requirements.size : 64000000;
    auto it = m_deviceMemoryTypeMap.find(typeIndex);
    if (it == m_deviceMemoryTypeMap.end()) {
        vk_buffer->m_allocation = AddAllocation(typeIndex, size);
    } else {
        for (auto tmp : it->second) {
            if (!tmp->FreeBlocks.empty()) {
                vk_buffer->m_allocation = tmp;
                break;
            }
        }
        if (vk_buffer->m_allocation == nullptr) {
            vk_buffer->m_allocation = AddAllocation(typeIndex, size);
        }
    }
    auto block = vk_buffer->m_allocation->Allocate(requirements.size, requirements.alignment);
    if (block.Size == 0) {
        vk_buffer->m_allocation = AddAllocation(typeIndex, size);
    }
    block = vk_buffer->m_allocation->Allocate(requirements.size, requirements.alignment);
    if (block.Size == 0) {
        HUH_ELOG(LogVulkanRHI, "Error Could Not Allocate Memory!")
        return false;
    }
    vk_buffer->m_allocator = this;
    vk_buffer->m_allocatedBlock = block;
    if (auto err = HUH::vkBindBufferMemory(*m_device, *vk_buffer, vk_buffer->m_allocation->Memory, block.Offset);
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Failed to bind memory Error: {}", err)
        return false;
    }
    return true;
}

bool VulkanMemoryAllocator::Allocate(Image* buffer, Type type) {
    const auto vk_image = dynamic_cast<VulkanImage*>(buffer);
    const auto requirements = vk_image->GetMemoryRequirements();
    const auto typeIndex = FindMemoryType(requirements, ConvertMemoryType(type));
    // TODO customizable base size
    const Uint32 size = requirements.size > 64000000 ? requirements.size : 64000000;
    auto it = m_deviceMemoryTypeMap.find(typeIndex);
    if (it == m_deviceMemoryTypeMap.end()) {
        vk_image->m_allocation = AddAllocation(typeIndex, size);
    } else {
        for (auto tmp : it->second) {
            if (!tmp->FreeBlocks.empty()) {
                vk_image->m_allocation = tmp;
                break;
            }
        }
        if (vk_image->m_allocation == nullptr) {
            vk_image->m_allocation = AddAllocation(typeIndex, size);
        }
    }
    auto block = vk_image->m_allocation->Allocate(requirements.size, requirements.alignment);
    if (block.Size == 0) {
        vk_image->m_allocation = AddAllocation(typeIndex, size);
        block = vk_image->m_allocation->Allocate(requirements.size, requirements.alignment);
    }
    if (block.Size == 0) {
        HUH_ELOG(LogVulkanRHI, "Error Could Not Allocate Memory!")
        return false;
    }
    vk_image->m_allocator = this;
    vk_image->m_allocatedBlock = block;
    if (auto err = HUH::vkBindImageMemory(*m_device, *vk_image, vk_image->m_allocation->Memory, block.Offset);
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Failed to bind memory Error: {}", err)
        return false;
    }
    return true;
}

bool VulkanMemoryAllocator::Free(Image* buffer) {
    const auto vk_buffer = dynamic_cast<VulkanImage*>(buffer);
    vk_buffer->m_allocation->Free(vk_buffer->m_allocatedBlock);
    return true;
}

bool VulkanMemoryAllocator::Free(Buffer* buffer) {
    const auto vk_buffer = dynamic_cast<VulkanBuffer*>(buffer);
    vk_buffer->m_allocation->Free(vk_buffer->m_allocatedBlock);
    return true;
}

VkMemoryPropertyFlags VulkanMemoryAllocator::ConvertMemoryType(MemoryAllocator::Type type) {
    VkMemoryPropertyFlags flags = 0;
    if (HUH::CheckAllFlag(type, RHI::MemoryAllocator::Type::Device | RHI::MemoryAllocator::Type::Host)) {
        flags |= VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        return flags;
    }
    if (HUH::CheckFlag(type, RHI::MemoryAllocator::Type::Device)) {
        flags |= VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }
    if (HUH::CheckFlag(type, RHI::MemoryAllocator::Type::Host)) {
        flags |= VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    }
    return flags;
}

Uint32 VulkanMemoryAllocator::FindMemoryType(VkMemoryRequirements requirements,
                                             VkMemoryPropertyFlags properties) const {
    for (uint32_t i = 0; i < m_device->m_memoryProperties.memoryTypeCount; i++) {
        if ((requirements.memoryTypeBits & (1 << i))
            && (m_device->m_memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw std::runtime_error("Not a valid VkMemoryPropertyFlagBits");
}

VulkanMemoryAllocator::Allocation* VulkanMemoryAllocator::AddAllocation(Uint32 memoryTypeIndex, Uint32 size) {
    VkMemoryAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = size,
        .memoryTypeIndex = memoryTypeIndex,
    };
    auto* allocation = new Allocation();
    allocation->FreeBlocks.emplace_back(0, size);
    if (auto err = HUH::vkAllocateMemory(*m_device, &allocInfo, nullptr, &allocation->Memory); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Failed to allocate memory allocation Error: {}", err)
        return nullptr;
    }
    auto it = m_deviceMemoryTypeMap.find(memoryTypeIndex);
    if (it == m_deviceMemoryTypeMap.end()) {
        m_deviceMemoryTypeMap.insert({memoryTypeIndex, {allocation}});
    } else {
        it->second.push_back(allocation);
    }
    return allocation;
}

VulkanMemoryAllocator::MemoryBlock VulkanMemoryAllocator::Allocation::Allocate(Uint32 size, Uint32 alignment) {
    MemoryBlock block{};
    for (auto it = FreeBlocks.begin(); it != FreeBlocks.end(); ++it) {
        if (it->Size >= size) {
            if (it->Offset % alignment == 0) {
                block.Offset = it->Offset;
                block.Size = size;
                if (it->Size - size != 0) {
                    it = ++FreeBlocks.insert(it, {it->Offset + size, it->Size - size});
                }
                FreeBlocks.erase(it);
                break;
            }
            if (it->Offset < alignment && it->Size - (alignment - it->Offset) >= size) {
                auto offset = alignment - it->Offset;
                block.Offset = it->Offset + offset;
                block.Size = size;
                it = ++FreeBlocks.insert(it, {it->Offset, offset});
                if (it->Size - offset - size != 0) {
                    it = ++FreeBlocks.insert(it, {it->Offset + offset + size, it->Size - offset - size});
                }
                FreeBlocks.erase(it);
                break;
            }
            if (it->Offset > alignment
                && it->Size - (it->Offset - ((it->Offset - (it->Offset / alignment) * alignment)) >= size)) {
                auto offset = (it->Offset - (it->Offset / alignment) * alignment);
                block.Offset = it->Offset - offset;
                block.Size = size;
                it = ++FreeBlocks.insert(it, {it->Offset, offset});
                if (it->Size - offset - size != 0) {
                    it = ++FreeBlocks.insert(it, {it->Offset + offset + size, it->Size - offset - size});
                }
                FreeBlocks.erase(it);
                break;
            }
        }
    }
    return block;
}

bool VulkanMemoryAllocator::Allocation::Free(MemoryBlock block) {
    // TODO: merging strategy ?
    FreeBlocks.push_back(block);
    std::sort(FreeBlocks.begin(), FreeBlocks.end(),
              [](MemoryBlock lht, MemoryBlock rht) { return lht.Offset < rht.Offset; });
    for (auto it = FreeBlocks.begin();;) {
        if (it == FreeBlocks.end() - 1 || it == FreeBlocks.end()) {
            break;
        }
        if ((it->Offset + it->Size) == (it + 1)->Offset) {
            it = FreeBlocks.insert(it, {it->Offset, it->Size + (it + 1)->Size});
            it = FreeBlocks.erase(++it);
            it = FreeBlocks.erase(it);
        }
        if (it == FreeBlocks.end() - 1 || it == FreeBlocks.end()) {
            break;
        }
        ++it;
    }
    return true;
}

VulkanMemoryAllocator::VulkanMemoryAllocator(VulkanDevice* device) : m_device(device) {
}

VulkanMemoryAllocator::~VulkanMemoryAllocator() {
    for (auto [type, vec] : m_deviceMemoryTypeMap) {
        for (auto alloc : vec) {
            HUH::vkFreeMemory(*m_device, alloc->Memory, nullptr);
            delete alloc;
            alloc = nullptr;
        }
    }
}

}// namespace HUH::RHI
