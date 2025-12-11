#include "HUH/Window/Linux/Wayland/definitions.h"

#include <cstring>
#include <HUH/Window/Linux/Wayland/window.h>
#include <wayland-client.h>
#include <bits/this_thread_sleep.h>
#include <wayland-protocols/xdg-shell-client-protocol.h>

namespace HUH {

wl_display* WaylandWindow::s_waylandDisplay = nullptr;
wl_registry* WaylandWindow::s_waylandRegistry = nullptr;
wl_compositor* WaylandWindow::s_waylandCompositor = nullptr;
xdg_wm_base* WaylandWindow::s_xdgWmBase = nullptr;
const wl_registry_listener WaylandWindow::s_registryListener = {
    .global = HandleWaylandGlobalRegister,
    .global_remove = HandleWaylandGlobalRegisterRemove,
};
const xdg_wm_base_listener WaylandWindow::s_xdgWmBaseListener = {.ping = xdgWmBasePing};
const xdg_surface_listener WaylandWindow::s_xdgSurfaceListener = {.configure = xdgSurfaceConfigure};

extern "C" WindowProto* CreateWindowImpl(const std::string& name) {
    return new WaylandWindow(name);
}

void HandleWaylandGlobalRegister(void* data,
                                 struct wl_registry* registry,
                                 uint32_t name,
                                 const char* interface,
                                 uint32_t version) {
#ifdef HUH_DEBUG
    HUH_ILOG(LogWaylandWindow, "Wayland Global Interface: {} Version: {} Name: {} ", interface, version, name);
#endif
    if (std::strcmp(interface, wl_compositor_interface.name) == 0) {
        WaylandWindow::s_waylandCompositor =
            static_cast<wl_compositor*>(wl_registry_bind(registry, name, &wl_compositor_interface, version));
    } else if (std::strcmp(interface, xdg_wm_base_interface.name) == 0) {
        WaylandWindow::s_xdgWmBase =
            static_cast<xdg_wm_base*>(wl_registry_bind(registry, name, &xdg_wm_base_interface, version));
        xdg_wm_base_add_listener(WaylandWindow::s_xdgWmBase, &WaylandWindow::s_xdgWmBaseListener, nullptr);
    }
}
void HandleWaylandGlobalRegisterRemove(void* data, struct wl_registry* registry, uint32_t name) {
}

void xdgWmBasePing(void* data, struct xdg_wm_base* xdg_wm_base, uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

void xdgSurfaceConfigure(void* data, struct xdg_surface* xdg_surface, uint32_t serial) {
    WaylandWindow* window = static_cast<WaylandWindow*>(data);
    xdg_surface_ack_configure(xdg_surface, serial);
    wl_surface_commit(window->m_surface);
}

WaylandWindow::WaylandWindow(const std::string& name) : WindowProto(name) {
    if (s_waylandDisplay == nullptr) {
        s_waylandDisplay = wl_display_connect(nullptr);
        if (!s_waylandDisplay) {
            HUH_ELOG(LogWaylandWindow, "Couldn't connect to wayland display");
            throw std::runtime_error("Couldn't connect to wayland display");
        }
        s_waylandRegistry = wl_display_get_registry(s_waylandDisplay);
        wl_registry_add_listener(s_waylandRegistry, &s_registryListener, nullptr);
        wl_display_roundtrip(s_waylandDisplay);
    }
    m_surface = wl_compositor_create_surface(s_waylandCompositor);
    m_xdgSurface = xdg_wm_base_get_xdg_surface(s_xdgWmBase, m_surface);
    xdg_surface_add_listener(m_xdgSurface, &s_xdgSurfaceListener, this);
    m_xdgToplevel = xdg_surface_get_toplevel(m_xdgSurface);
    xdg_toplevel_set_title(m_xdgToplevel, m_name.c_str());
    wl_surface_commit(m_surface);
}

WaylandWindow::~WaylandWindow() {
    if (s_waylandDisplay) {
        HUH_ILOG(LogWaylandWindow, "Destroying Wayland Display connection")
        wl_display_disconnect(s_waylandDisplay);
    }
}

void WaylandWindow::Show() {
    HUH_ILOG(LogWaylandWindow, "WaylandWindow::Show");
}

void WaylandWindow::Loop() {
    while (wl_display_dispatch(s_waylandDisplay)) {
        /* This space deliberately left blank */
    }
}

}// namespace HUH