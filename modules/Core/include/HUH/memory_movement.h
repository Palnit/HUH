#pragma once

#include "logging.h"

#include <HUH/concepts.h>
#include <HUH/types.h>

#include <cstring>

namespace HUH {

template<typename Type>
    requires(ZeroConstructable<Type>)
HUH_FORCE_INLINE void DefaultConstruct(void* ptr, const size_t size) {
    std::memset(ptr, 0, sizeof(Type) * size);
}

template<typename Type>
    requires(DefaultConstructable<Type>)
HUH_FORCE_INLINE void DefaultConstruct(void* ptr, const size_t size) {
    Type* typePtr = (Type*)ptr;
    for (size_t i = 0; i < size; ++i) {
        (void)::new (typePtr) Type;
        ++typePtr;
    }
}

template<typename Type>
    requires(TriviallyDestructable<Type>)
HUH_FORCE_INLINE void DefaultDestruct(Type* ptr, size_t size) {
}

template<typename Type>
HUH_FORCE_INLINE void DefaultDestruct(Type* ptr, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        ptr->~Type();
        ++ptr;
    }
}

template<typename Type>
    requires(ZeroConstructable<Type>)
HUH_FORCE_INLINE bool CompareArrays(Type* ptrA, Type* ptrB, const size_t size) {
    return !size || !std::memcmp(ptrA, ptrB, size);
}

template<typename Type>
HUH_FORCE_INLINE bool CompareArrays(Type* ptrA, Type* ptrB, const size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (!(*ptrA == *ptrB)) {
            return false;
        }
        ++ptrA;
        ++ptrB;
    }
    return true;
}

}// namespace HUH