#pragma once

#include "wayland-protocols/xdg-shell-client-protocol.h"

#include <HUH/definitions.h>
#include <HUH/Window/prototypes/window_proto.h>

struct wl_display;
struct wl_registry;
struct wl_registry_listener;
struct wl_compositor;
struct wl_surface;
struct xdg_wm_base;
struct xdg_wm_base_listener;
struct xdg_surface;
struct xdg_toplevel;
struct xdg_surface_listener;

namespace HUH {

extern "C" WindowProto* CreateWindowImpl(const std::string& name);

HUH_WAYLANDWINDOW_API void HandleWaylandGlobalRegister(void* data,
                                                       wl_registry* registry,
                                                       uint32_t name,
                                                       const char* interface,
                                                       uint32_t version);

HUH_WAYLANDWINDOW_API void HandleWaylandGlobalRegisterRemove(void* data, wl_registry* registry, uint32_t name);
HUH_WAYLANDWINDOW_API void xdgWmBasePing(void* data, struct xdg_wm_base* xdg_wm_base, uint32_t serial);

HUH_WAYLANDWINDOW_API void xdgSurfaceConfigure(void* data, struct xdg_surface* xdg_surface, uint32_t serial);

class HUH_WAYLANDWINDOW_API WaylandWindow : public WindowProto {
public:
    WaylandWindow(const std::string& name);
    ~WaylandWindow() override;

    friend WindowProto* CreateWindowImpl(const std::string& name);
    void Show() override;
    void Loop() override;

protected:
    HUH_WAYLANDWINDOW_API friend void HandleWaylandGlobalRegister(void* data,
                                                                  wl_registry* registry,
                                                                  uint32_t name,
                                                                  const char* interface,
                                                                  uint32_t version);
    HUH_WAYLANDWINDOW_API friend void HandleWaylandGlobalRegisterRemove(void* data,
                                                                        wl_registry* registry,
                                                                        uint32_t name);

    HUH_WAYLANDWINDOW_API friend void xdgSurfaceConfigure(void* data, struct xdg_surface* xdg_surface, uint32_t serial);

    wl_surface* m_surface;
    xdg_surface* m_xdgSurface;
    xdg_toplevel* m_xdgToplevel;
    static wl_compositor* s_waylandCompositor;
    static wl_display* s_waylandDisplay;
    static wl_registry* s_waylandRegistry;
    static xdg_wm_base* s_xdgWmBase;
    static const wl_registry_listener s_registryListener;
    static const xdg_wm_base_listener s_xdgWmBaseListener;
    static const xdg_surface_listener s_xdgSurfaceListener;
};
}// namespace HUH
