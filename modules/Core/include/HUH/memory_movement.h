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
    requires(DefaultConstructable<Type> && !ZeroConstructable<Type>)
HUH_FORCE_INLINE void DefaultConstruct(void* ptr, const size_t size) {
    Type* typePtr = static_cast<Type*>(ptr);
    for (size_t i = 0; i < size; ++i) {
        (void)::new (typePtr) Type;
        ++typePtr;
    }
}

template<typename DstType, typename SrcType>
    requires(TrivialCopyConstructible<DstType> && Same<DstType, SrcType>)
HUH_FORCE_INLINE void DefaultCopy(void* dst, const SrcType* src, const size_t size) {
    std::memcpy(dst, src, sizeof(SrcType) * size);
}

template<typename DstType, typename SrcType>
HUH_FORCE_INLINE void DefaultCopy(void* dst, const SrcType* src, const size_t size) {
    for (size_t i = 0; i < size; ++i) {
        ::new (dst) DstType(*src);
        ++static_cast<DstType*&>(dst);
        ++src;
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

template<typename DestType, typename SrcType>
    requires(Same<DestType, SrcType> || (ZeroConstructable<DestType> && TriviallyDestructable<SrcType>))
HUH_FORCE_INLINE void DefaultMove(void* dst, SrcType* src, const size_t size) {
    std::memmove(dst, src, sizeof(SrcType) * size);
}

template<typename DstType, typename SrcType>
HUH_FORCE_INLINE void DefaultMove(void* dst, SrcType* src, const size_t size) {
    for (size_t i = 0; i < size; ++i) {
        ::new (dst) DstType(std::move(*src));
        ++static_cast<DstType*&>(dst);
        (src++)->~SrcType();
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