#pragma once
#include <HUH/definitions.h>
#include <windows.h>
#include <libloaderapi.h>
#include <string>

namespace HUH {
class HUH_API DynamicLibrary {
public:
    explicit DynamicLibrary(const std::string& path);

    template<typename FuncType>
    FuncType* GetExport(const std::string& name) {
        return reinterpret_cast<FuncType*>(GetProcAddress(m_handle, name.c_str()));
    }
    [[nodiscard]] bool IsLoaded() const { return m_handle != nullptr; }
    static std::string GetErrorMessage();
    ~DynamicLibrary();

private:
    HMODULE m_handle = nullptr;
};
}// namespace HUH