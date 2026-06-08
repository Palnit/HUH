#pragma once

#include "HUH/RHI/Types/buffer.h"

#include <HUH/RHI/memory_allocator.h>
#include <HUH/RHI/vulkan/fwd.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>

#include <HUH/types.h>

#include <vector>

namespace HUH::RHI {
class VulkanMemoryAllocator : public MemoryAllocator {
public:
    friend class VulkanDevice;
    void Destroy() override;
    bool Allocate(Buffer* buffer, Type type) override;
    bool Allocate(Image* buffer, Type type) override;
    bool Free(Buffer* buffer) override;
    bool Free(Image* buffer) override;

    struct MemoryBlock {
        Uint32 Offset = 0;
        Uint32 Size = 0;
    };

    struct Allocation {
        friend class VulkanBuffer;
        VkDeviceMemory Memory = nullptr;

        MemoryBlock Allocate(Uint32 size, Uint32 alignment);
        ~Allocation();
        bool Free(MemoryBlock block);

        Buffer::SharedMemoryInfo::PlatformHandle GetPlatformHandle();

        Buffer::SharedMemoryInfo::PlatformHandle Handle;
        std::vector<MemoryBlock> FreeBlocks;
        VulkanMemoryAllocator* m_parent = nullptr;
    };

protected:
    VulkanMemoryAllocator(VulkanDevice* m_device);
    ~VulkanMemoryAllocator() override;

    static VkMemoryPropertyFlags ConvertMemoryType(MemoryAllocator::Type type);
    HUH_NODISCARD Uint32 FindMemoryType(VkMemoryRequirements requirements, VkMemoryPropertyFlags properties) const;
    VulkanMemoryAllocator::Allocation* AddAllocation(Uint32 memoryTypeIndex, Uint32 size);
    VulkanDevice* m_device;
    std::map<Uint32, std::vector<Allocation*>> m_deviceMemoryTypeMap;
};
}// namespace HUH::RHI
