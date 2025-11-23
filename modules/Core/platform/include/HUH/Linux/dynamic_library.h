#pragma once
#include <HUH/definitions.h>
#include <dlfcn.h>
#include <string>

namespace HUH {
class HUH_CORE_API DynamicLibrary {
public:
    DynamicLibrary() = default;
    explicit DynamicLibrary(const std::string& path);

    bool Load(const std::string& path);
    void Unload();

    template<typename FuncType>
    HUH_NODISCARD HUH_FORCE_INLINE FuncType* GetExport(const std::string& name) {
        return (FuncType*)(dlsym(m_handle, name.c_str()));
    }
    HUH_NODISCARD bool IsLoaded() const;
    static std::string GetErrorMessage();
    HUH_NODISCARD static std::string DecoratePlatformLibraryName(const std::string& name);
    ~DynamicLibrary();

private:
    void* m_handle = nullptr;
};
}// namespace HUH