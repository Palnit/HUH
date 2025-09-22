#include <HUH/Windows/dynamic_library.h>

namespace HUH {
DynamicLibrary::DynamicLibrary(const std::string& path) { m_handle =
                                                              dlopen(path.c_str(),
                                                                     RTLD_LAZY
                                                                         | RTLD_GLOBAL);
}
std::string DynamicLibrary::GetErrorMessage() { return dlerror(); }
DynamicLibrary::~DynamicLibrary() {
    if (m_handle != nullptr) { dlclose(m_handle); }
}

}// namespace HUH