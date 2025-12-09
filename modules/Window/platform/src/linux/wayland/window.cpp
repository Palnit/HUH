#include <HUH/Window/definitions.h>

#include <HUH/Window/Linux/Wayland/window.h>
namespace HUH {

extern "C" WindowProto* CreateWindowImpl(const std::string& name) {
    return new WaylandWindow(name);
}

WaylandWindow::WaylandWindow(const std::string& name) : WindowProto(name) {
}

void WaylandWindow::Show() {
    HUH_ILOG(LogWindow, "WaylandWindow::Show");
}

void WaylandWindow::Loop() {
}

}// namespace HUH