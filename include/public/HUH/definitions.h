#pragma once

#if defined(_MSC_VER)
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#define HUH_FORCE_INLINE __forceinline
#include <intrin.h>
#define HUH_POP_COUNT64(bits) __popcnt64(bits)
#define HUH_POP_COUNT32(bits) __popcnt(bits)
#elif defined(__GNUC__)
#define EXPORT __attribute__((visibility("default")))
#define HUH_FORCE_INLINE __attribute__((always_inline)) inline
#define IMPORT
#define HUH_POP_COUNT64(bits) __builtin_popcountll(bits)
#define HUH_POP_COUNT32(bits) __builtin_popcount(bits)
#else
#define EXPORT
#define IMPORT
#define HUH_FORCE_INLINE inline
#define HUH_POP_COUNT64(bits) 0
#define HUH_POP_COUNT32(bits) 0
#pragma warning Unknown dynamic link import / export semantics.
#endif

#define HUH_NODISCARD [[nodiscard]]
#define HUH_CONSTEXPR_FORCE HUH_FORCE_INLINE constexpr

#include <HUH/lib_defines.h>
