#pragma once

#include <HUH/concepts.h>
#include <HUH/definitions.h>
#include <HUH/lib_defines.h>
#include <HUH/types.h>

namespace HUH {

template<typename Type>
concept Allocator = requires(Type type, void* ptr, size_t size) {
    { type.Allocate(size) } -> Same<void*>;
    { type.Reallocate(ptr, size) } -> Same<void*>;
    { type.Deallocate(ptr) } -> Same<void>;
};

class HUH_CORE_API BaseAllocator {
public:
    virtual void* Allocate(size_t size) = 0;
    virtual void* Reallocate(void* ptr, size_t size) = 0;
    virtual void Deallocate(void* ptr) = 0;
    virtual ~BaseAllocator() = default;
};

}// namespace HUH