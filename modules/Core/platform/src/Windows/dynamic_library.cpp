#include <HUH/Windows/dynamic_library.h>
#include <errhandlingapi.h>

namespace HUH {
DynamicLibrary::DynamicLibrary(const std::string& path) { m_handle = LoadLibrary(path.c_str()); }
std::string DynamicLibrary::GetErrorMessage() {
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) { return std::string(); }

    LPSTR messageBuffer = nullptr;

    size_t size =
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
                      errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    LocalFree(messageBuffer);

    return message;
}
DynamicLibrary::~DynamicLibrary() { FreeLibrary(m_handle); }

}// namespace HUH