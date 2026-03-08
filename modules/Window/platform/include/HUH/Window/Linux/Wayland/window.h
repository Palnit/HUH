#pragma once

#include "wayland-protocols/xdg-shell-client-protocol.h"
#include "wayland-protocols/xdg-decoration-client-protocol.h"

#include <HUH/event.h>
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
struct xdg_toplevel_listener;
struct wl_array;

namespace HUH {

extern "C" WindowProto* CreateWindowImpl(const std::string& name, Int32 width, Int32 height);

HUH_WAYLANDWINDOW_API void HandleWaylandGlobalRegister(void* data,
                                                       wl_registry* registry,
                                                       Uint32 name,
                                                       const char* interface,
                                                       Uint32 version);

HUH_WAYLANDWINDOW_API void HandleWaylandGlobalRegisterRemove(void* data, wl_registry* registry, Uint32 name);
HUH_WAYLANDWINDOW_API void xdgWmBasePing(void* data, xdg_wm_base* xdg_wm_base, Uint32 serial);

HUH_WAYLANDWINDOW_API void xdgSurfaceConfigure(void* data, xdg_surface* xdg_surface, Uint32 serial);

HUH_WAYLANDWINDOW_API void xdgToplevelConfigure(void* data,
                                                xdg_toplevel* xdg_toplevel,
                                                int32_t width,
                                                int32_t height,
                                                wl_array* states);

HUH_WAYLANDWINDOW_API void xdgToplevelClose(void* data, struct xdg_toplevel* toplevel);

void xdgToplevelConfigureBounds(void* data, struct xdg_toplevel* xdg_toplevel, int32_t width, int32_t height);

void xdgToplevelWmCapabilities(void* data, xdg_toplevel* xdg_toplevel, struct wl_array* capabilities);

class HUH_WAYLANDWINDOW_API WaylandWindow : public WindowProto {
public:
    WaylandWindow(const std::string& name, const Int32 width, const Int32 height);
    ~WaylandWindow() override;

    friend WindowProto* CreateWindowImpl(const std::string& name);
    void Show() override;
    bool Loop() override;

protected:
    HUH_WAYLANDWINDOW_API friend void HandleWaylandGlobalRegister(void* data,
                                                                  wl_registry* registry,
                                                                  Uint32 name,
                                                                  const char* interface,
                                                                  Uint32 version);
    HUH_WAYLANDWINDOW_API friend void HandleWaylandGlobalRegisterRemove(void* data, wl_registry* registry, Uint32 name);

    HUH_WAYLANDWINDOW_API friend void xdgSurfaceConfigure(void* data, struct xdg_surface* xdg_surface, Uint32 serial);

    HUH_WAYLANDWINDOW_API friend void xdgToplevelConfigure(void* data,
                                                           xdg_toplevel* xdg_toplevel,
                                                           int32_t width,
                                                           int32_t height,
                                                           wl_array* states);

    HUH_WAYLANDWINDOW_API friend void xdgToplevelClose(void* data, struct xdg_toplevel* toplevel);

    wl_surface* m_surface;
    xdg_surface* m_xdgSurface;
    xdg_toplevel* m_xdgToplevel;
    zxdg_toplevel_decoration_v1* m_zxdgToplevelDecoration;
    bool close = false;
    static wl_compositor* s_waylandCompositor;
    static wl_display* s_waylandDisplay;
    static wl_registry* s_waylandRegistry;
    static xdg_wm_base* s_xdgWmBase;
    static zxdg_decoration_manager_v1* s_zxdgDecorationManager;
    static const wl_registry_listener s_registryListener;
    static const xdg_wm_base_listener s_xdgWmBaseListener;
    static const xdg_surface_listener s_xdgSurfaceListener;
    static const xdg_toplevel_listener s_xdgToplevelListener;
};
}// namespace HUH
