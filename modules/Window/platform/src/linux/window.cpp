#include <HUH/Window/Linux/window.h>
#include <HUH/Window/definitions.h>

namespace HUH {

DynamicLibrary Window::s_lib;
Window::Window(const std::string& name, const HUH::Vector2u32 size) : WindowProto(name, size) {
    // Look into magic so we can change the impl into morphing the 'this' pointer if possible
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
    m_impl = s_createImpl(name, size);
    // Rethrowing events
    m_impl->OnClose.Add([&](Window*) { this->OnClose(this); });
    m_impl->OnSizeChange.Add([&](Window*, const HUH::Vector2u32 inSize) {
        this->m_size = inSize;
        this->OnSizeChange.ExecuteAll(this, inSize);
    });

    m_impl->OnMouseEnter.Add([&](Window*, HUH::Vector2d pos) { this->OnMouseEnter(this, pos); });
    m_impl->OnMouseLeave.Add([&](Window*) { this->OnMouseLeave(this); });
    m_impl->OnMouseMove.Add([&](Window*, HUH::Vector2d pos) { this->OnMouseMove(this, pos); });
    m_impl->OnMousePress.Add([&](Window*, KeyBindings bindings) { this->OnMousePress(this, bindings); });
    m_impl->OnMouseRelease.Add([&](Window*, KeyBindings bindings) { this->OnMouseRelease(this, bindings); });

    m_impl->OnKeyPress.Add(
        [&](Window*, KeyBindings bindings, const std::string& utf8) { this->OnKeyPress(this, bindings, utf8); });
    m_impl->OnKeyRelease.Add([&](Window*, KeyBindings bindings) { this->OnKeyRelease(this, bindings); });
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

bool Window::Loop() {
    return m_impl->Loop();
}

}// namespace HUH
