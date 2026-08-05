#pragma once

#include <HUH/definitions.h>
#include <cstdlib>

namespace HUH {
// TODO make an allocator without the use of pre defined malloc and make it thread safe similar to google's
class HUH_CORE_API MallocAllocator {
public:
    HUH_FORCE_INLINE static void* Allocate(const size_t size) { return std::malloc(size); }
    HUH_FORCE_INLINE static void* Reallocate(void* ptr, const size_t size) { return std::realloc(ptr, size); }
    HUH_FORCE_INLINE static void Deallocate(void* ptr) { std::free(ptr); }
    ~MallocAllocator() = default;
};
}// namespace HUH