#pragma once

#if defined(_MSC_VER)
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#define HUH_FORCE_INLINE __forceinline
#elif defined(__GNUC__)
#define EXPORT __attribute__((visibility("default")))
#define HUH_FORCE_INLINE __attribute__((always_inline)) inline
#define IMPORT
#else
#define EXPORT
#define IMPORT
#define HUH_FORCE_INLINE inline
#pragma warning Unknown dynamic link import / export semantics.
#endif

#if defined(HUH_COMP)
#define HUH_API EXPORT
#else
#define HUH_API IMPORT
#endif

#define HUH_NODISCARD [[nodiscard]]
#define HUH_CONSTEXPR_FORCE HUH_FORCE_INLINE constexpr
