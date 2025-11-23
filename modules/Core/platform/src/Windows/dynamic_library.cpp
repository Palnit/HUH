#include <HUH/Windows/dynamic_library.h>
#include <errhandlingapi.h>
#include <libloaderapi.h>

namespace HUH {
DynamicLibrary::DynamicLibrary(const std::string& path) {
    Load(path);
}
std::string DynamicLibrary::GetErrorMessage() {
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return {};
    }

    LPSTR messageBuffer = nullptr;

    size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                                    | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
                                nullptr, errorMessageID, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                                (LPSTR)&messageBuffer, 0, nullptr);

    std::string message(messageBuffer, size);

    LocalFree(messageBuffer);

    return message;
}
DynamicLibrary::~DynamicLibrary() {
    Unload();
}

std::string DynamicLibrary::DecoratePlatformLibraryName(const std::string& name) {
    return name + ".dll";
}

bool DynamicLibrary::Load(const std::string& path) {
    Unload();
    m_handle = LoadLibrary(path.c_str());
    return IsLoaded();
}

void DynamicLibrary::Unload() {
    if (IsLoaded()) {
        FreeLibrary(m_handle);
        m_handle = nullptr;
    }
}

}// namespace HUH