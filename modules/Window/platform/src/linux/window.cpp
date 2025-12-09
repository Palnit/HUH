#include <HUH/Window/Linux/window.h>
#include <HUH/Window/definitions.h>

namespace HUH {
#define CallFunction(FuncName, ...)\
    m_impl ? m_impl->FuncName(__VA_ARGS__) : HUH_ELOG(LogWindow, "Couldn't run function: {}() no window implementation",#FuncName);

DynamicLibrary Window::s_lib;
Window::Window(const std::string& name) : WindowProto(name) {
    if (!s_lib.IsLoaded() || s_createImpl == nullptr) {
        const std::string sessionType = std::getenv("XDG_SESSION_TYPE");
        if (sessionType == "wayland") {
            if (!s_lib.Load(HUH::DynamicLibrary::DecoratePlatformLibraryName("HUH-WaylandWindow"))) {
                HUH_ELOG(LogWindow, "Could not load HUH wayland shared library: {}",
                         HUH::DynamicLibrary::GetErrorMessage())
                return;
            }
        }
        s_createImpl = s_lib.GetExport<CreateWindowImpl>("CreateWindowImpl");
        if (s_createImpl == nullptr) {
            HUH_ELOG(LogWindow, "Could not load CreateWindowImpl()");
            return;
        }
    }
    m_impl = s_createImpl(name);
}

void Window::Show() {
    CallFunction(Show)
}

void Window::Loop() {
    CallFunction(Loop)
}

}// namespace HUH
