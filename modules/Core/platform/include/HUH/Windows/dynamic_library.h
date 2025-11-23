#pragma once
#include <HUH/definitions.h>
#include <windows.h>
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
        return reinterpret_cast<FuncType*>(GetProcAddress(m_handle, name.c_str()));
    }
    [[nodiscard]] bool IsLoaded() const { return m_handle != nullptr; }
    static std::string GetErrorMessage();
    HUH_NODISCARD static std::string DecoratePlatformLibraryName(const std::string& name);
    ~DynamicLibrary();

private:
    HMODULE m_handle = nullptr;
};
}// namespace HUH