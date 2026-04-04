#pragma once

#include <HUH/RHI/memory_allocator.h>
#include <HUH/RHI/vulkan/fwd.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>

namespace HUH::RHI {
class VulkanMemoryAllocator : public MemoryAllocator {
public:
    friend class VulkanDevice;
    void Destroy() override;
    bool Allocate(Buffer* buffer, Type type) override;
    bool Free(Buffer* buffer) override;

    static VkMemoryPropertyFlags ConvertMemoryType(MemoryAllocator::Type type);
    Uint64 FindMemoryType(VkMemoryRequirements requirements, VkMemoryPropertyFlags properties);

    class MemoryBlock {
        Uint64 offset = 0;
        Uint64 size = 0;
    };

    class Allocation {
        VkDeviceMemory memory = nullptr;
        Uint64 Size = 0;
        Uint64 Offset = 0;

        MemoryBlock Allocate(Uint64 size, Uint64 alignment);
        bool Free(MemoryBlock block);

        std::vector<MemoryBlock> FreeBlocks;
    };

protected:
    VulkanMemoryAllocator(VulkanDevice* m_device);
    ~VulkanMemoryAllocator() override;
    VulkanDevice* m_device;
    std::map<Uint64, std::vector<Allocation>> m_deviceMemoryTypeMap;
};
}// namespace HUH::RHI
