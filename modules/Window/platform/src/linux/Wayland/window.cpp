#include "HUH/Window/Linux/Wayland/definitions.h"

#include <cstring>
#include <HUH/Window/Linux/Wayland/window.h>

#include "wayland-protocols/xdg-decoration-client-protocol.h"

#include <wayland-client.h>
#include <bits/this_thread_sleep.h>
#include <wayland-protocols/xdg-shell-client-protocol.h>

namespace HUH {

wl_display* WaylandWindow::s_waylandDisplay = nullptr;
wl_registry* WaylandWindow::s_waylandRegistry = nullptr;
wl_compositor* WaylandWindow::s_waylandCompositor = nullptr;
xdg_wm_base* WaylandWindow::s_xdgWmBase = nullptr;
zxdg_decoration_manager_v1* WaylandWindow::s_zxdgDecorationManager = nullptr;
const wl_registry_listener WaylandWindow::s_registryListener = {
    .global = HandleWaylandGlobalRegister,
    .global_remove = HandleWaylandGlobalRegisterRemove,
};
const xdg_wm_base_listener WaylandWindow::s_xdgWmBaseListener = {.ping = xdgWmBasePing};
const xdg_surface_listener WaylandWindow::s_xdgSurfaceListener = {.configure = xdgSurfaceConfigure};
const xdg_toplevel_listener WaylandWindow::s_xdgToplevelListener = {
    .configure = xdgToplevelConfigure,
    .close = xdgToplevelClose,
    .configure_bounds = xdgToplevelConfigureBounds,
    .wm_capabilities = xdgToplevelWmCapabilities,
};

extern "C" WindowProto* CreateWindowImpl(const std::string& name, Int32 width, Int32 height) {
    return new WaylandWindow(name, width, height);
}

void HandleWaylandGlobalRegister(void* data,
                                 wl_registry* registry,
                                 Uint32 name,
                                 const char* interface,
                                 Uint32 version) {
#ifdef HUH_DEBUG
    HUH_LOG(LogWaylandWindow, Logging::DebugLog, "Wayland Global Interface: {} Version: {} Name: {} ", interface,
            version, name);
#endif
    if (std::strcmp(interface, wl_compositor_interface.name) == 0) {
        WaylandWindow::s_waylandCompositor =
            static_cast<wl_compositor*>(wl_registry_bind(registry, name, &wl_compositor_interface, version));
    } else if (std::strcmp(interface, xdg_wm_base_interface.name) == 0) {
        WaylandWindow::s_xdgWmBase =
            static_cast<xdg_wm_base*>(wl_registry_bind(registry, name, &xdg_wm_base_interface, version));
        xdg_wm_base_add_listener(WaylandWindow::s_xdgWmBase, &WaylandWindow::s_xdgWmBaseListener, nullptr);
    } else if (std::strcmp(interface, zxdg_decoration_manager_v1_interface.name) == 0) {
        WaylandWindow::s_zxdgDecorationManager = static_cast<zxdg_decoration_manager_v1*>(
            wl_registry_bind(registry, name, &zxdg_decoration_manager_v1_interface, version));
    }
}

void HandleWaylandGlobalRegisterRemove(void* data, wl_registry* registry, Uint32 name) {
}

void xdgWmBasePing(void* data, xdg_wm_base* xdg_wm_base, const Uint32 serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

void xdgSurfaceConfigure(void* data, xdg_surface* xdg_surface, const Uint32 serial) {
    const auto* window = static_cast<WaylandWindow*>(data);
    xdg_surface_ack_configure(xdg_surface, serial);
    wl_surface_commit(window->m_surface);
}

void xdgToplevelConfigure(void* data, xdg_toplevel* xdg_toplevel, int32_t width, int32_t height, wl_array* states) {
    const auto window = static_cast<WaylandWindow*>(data);
    if (width == 0 || height == 0) {
        return;
    }
    window->m_width = width;
    window->m_height = height;
    window->OnSizeChange.ExecuteAll(nullptr, window->m_width, window->m_height);
}

void xdgToplevelClose(void* data, struct xdg_toplevel* toplevel) {
    const auto window = static_cast<WaylandWindow*>(data);
    window->OnClose.ExecuteAll(nullptr);
    window->close = true;
}

void xdgToplevelConfigureBounds(void* data, struct xdg_toplevel* xdg_toplevel, int32_t width, int32_t height) {
}

void xdgToplevelWmCapabilities(void* data, xdg_toplevel* xdg_toplevel, struct wl_array* capabilities) {
}

WaylandWindow::WaylandWindow(const std::string& name, const Int32 width, const Int32 height)
    : WindowProto(name, width, height) {
    if (s_waylandDisplay == nullptr) {
        s_waylandDisplay = wl_display_connect(nullptr);
        if (!s_waylandDisplay) {
            HUH_ELOG(LogWaylandWindow, "Couldn't connect to Wayland display");
            throw std::runtime_error("Couldn't connect to Wayland display");
        }
        s_waylandRegistry = wl_display_get_registry(s_waylandDisplay);
        wl_registry_add_listener(s_waylandRegistry, &s_registryListener, nullptr);
        wl_display_roundtrip(s_waylandDisplay);
    }
    m_surface = wl_compositor_create_surface(s_waylandCompositor);
    m_xdgSurface = xdg_wm_base_get_xdg_surface(s_xdgWmBase, m_surface);
    xdg_surface_add_listener(m_xdgSurface, &s_xdgSurfaceListener, this);
    m_xdgToplevel = xdg_surface_get_toplevel(m_xdgSurface);
    m_zxdgToplevelDecoration =
        zxdg_decoration_manager_v1_get_toplevel_decoration(s_zxdgDecorationManager, m_xdgToplevel);
    zxdg_toplevel_decoration_v1_set_mode(m_zxdgToplevelDecoration, ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
    xdg_toplevel_add_listener(m_xdgToplevel, &s_xdgToplevelListener, this);
    xdg_toplevel_set_min_size(m_xdgToplevel, m_width, m_height);
    // xdg_toplevel_set_max_size(m_xdgToplevel, m_width, m_height);
    xdg_toplevel_set_title(m_xdgToplevel, m_name.c_str());
    wl_surface_commit(m_surface);
    m_platform.WaylandDisplay = s_waylandDisplay;
    m_platform.WaylandSurface = m_surface;
}

WaylandWindow::~WaylandWindow() {
    if (s_waylandDisplay) {
        HUH_ILOG(LogWaylandWindow, "Destroying Wayland Display connection")
        zxdg_toplevel_decoration_v1_destroy(m_zxdgToplevelDecoration);
        xdg_toplevel_destroy(m_xdgToplevel);
        xdg_surface_destroy(m_xdgSurface);
        wl_surface_destroy(m_surface);
        wl_display_disconnect(s_waylandDisplay);
    }
}

void WaylandWindow::Show() {
    HUH_ILOG(LogWaylandWindow, "WaylandWindow::Show");
}

bool WaylandWindow::Loop() {
    return wl_display_dispatch(s_waylandDisplay) != -1 && close == false;
}

}// namespace HUH