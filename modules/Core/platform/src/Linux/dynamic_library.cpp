#include <HUH/Linux/dynamic_library.h>

namespace HUH {
DynamicLibrary::DynamicLibrary(const std::string& path) { Load(path); }

bool DynamicLibrary::Load(const std::string& path) {
#ifdef HUH_SHARED_LIB_LOAD_WITH_RELATIVE_PATH
    m_handle = dlopen(("./" + path).c_str(), RTLD_LAZY | RTLD_GLOBAL);
#else
    m_handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
#endif

    return IsLoaded();
}
void DynamicLibrary::Unload() {
    if (m_handle != nullptr) {
        dlclose(m_handle);
        m_handle = nullptr;
    }
}
bool DynamicLibrary::IsLoaded() const { return m_handle != nullptr; }
std::string DynamicLibrary::GetErrorMessage() { return dlerror(); }
std::string DynamicLibrary::DecoratePlatformLibraryName(const std::string& name) { return "lib" + name + ".so"; }
DynamicLibrary::~DynamicLibrary() { Unload(); }

}// namespace HUH