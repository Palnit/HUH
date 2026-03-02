#include <HUH/Window/Linux/window.h>
#include <HUH/Window/definitions.h>

namespace HUH {

DynamicLibrary Window::s_lib;
Window::Window(const std::string& name, const Int32 width, const Int32 height) : WindowProto(name, width, height) {
    if (!s_lib.IsLoaded() || s_createImpl == nullptr) {
        const std::string sessionType = std::getenv("XDG_SESSION_TYPE");
        if (sessionType == "wayland") {
            if (!s_lib.Load(HUH::DynamicLibrary::DecoratePlatformLibraryName("HUH-WaylandWindow"))) {
                HUH_ELOG(LogWindow, "Could not load HUH wayland shared library: {}",
                         HUH::DynamicLibrary::GetErrorMessage())
                throw std::runtime_error("Could not load HUH wayland shared library");
            }
        }
        s_createImpl = s_lib.GetExport<CreateWindowImpl>("CreateWindowImpl");
        if (s_createImpl == nullptr) {
            HUH_ELOG(LogWindow, "Could not load CreateWindowImpl()");
            throw std::runtime_error("Could not load CreateWindowImpl()");
        }
    }
    m_impl = s_createImpl(name, width, height);
    // Rethrowing events
    m_impl->OnClose.Add([&](Window*) { this->OnClose(this); });
    m_impl->OnSizeChange.Add([&](Window*, const int32_t inWidth, const int32_t inHeight) {
        this->m_height = inHeight;
        this->m_width = inWidth;
        this->OnSizeChange.ExecuteAll(this, inWidth, inHeight);
    });
}
Window::~Window() {
    delete m_impl;
}

const WindowProto::PlatformVariables& Window::GetPlatformVariables() const {
    return m_impl->GetPlatformVariables();
}
void Window::Show() {
    m_impl->Show();
}

void Window::Loop() {
    m_impl->Loop();
}

}// namespace HUH
