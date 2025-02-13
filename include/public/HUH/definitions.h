#ifndef HUH_INCLUDE_PUBLIC_HUH_HUH_DEFS_H_
#define HUH_INCLUDE_PUBLIC_HUH_HUH_DEFS_H_

#if defined(_MSC_VER)
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#else
    #define EXPORT
    #define IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

#if defined(HUH_COMP)
#   define HUH_API EXPORT
#else
#   define HUH_API IMPORT
#endif


#endif //HUH_INCLUDE_PUBLIC_HUH_HUH_DEFS_H_
