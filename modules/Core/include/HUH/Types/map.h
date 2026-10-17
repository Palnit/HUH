#pragma once

#include <HUH/Types/array.h>

#include <HUH/Types/Allocators/base_allocator.h>
#include <HUH/Types/Allocators/malloc_allocator.h>

namespace HUH {

template<typename KeyType, typename ValueType, HUH::Allocator AllocatorType = MallocAllocator>
class Map {

public:
    static constexpr size_t GroupWidth = 16;
    static constexpr size_t StartingCapacity = 4;

    enum class SlotType : HUH::Uint8 {
        Empty = 0x80,
        Deleted = 0xFE,
    };

private:
    HUH::Array<KeyType, AllocatorType> m_keys;
    HUH::Array<KeyType, AllocatorType> m_values;
    HUH::Array<Uint8, AllocatorType> m_control = HUH::Array(GroupWidth * 4, static_cast<HUH::Uint8>(SlotType::Empty));
};

}// namespace HUH