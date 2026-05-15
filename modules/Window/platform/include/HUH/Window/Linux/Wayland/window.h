#pragma once

#include <HUH/Window/prototypes/window_proto.h>
#include <HUH/definitions.h>
#include <HUH/enum_define.h>
#include <wayland-client-protocol.h>

struct wl_seat;
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
struct zxdg_toplevel_decoration_v1;
struct zxdg_decoration_manager_v1;

struct xkb_state;
struct xkb_context;
struct xkb_keymap;

namespace HUH {

extern "C" WindowProto* CreateWindowImpl(const std::string& name, const HUH::Vector2u32& size);

class HUH_WAYLANDWINDOW_API WaylandWindow : public WindowProto {
public:
    WaylandWindow(const std::string& name, HUH::Vector2u32 size);
    ~WaylandWindow() override;

    friend WindowProto* CreateWindowImpl(const std::string& name);
    void Show() override;
    bool Loop() override;

    enum class PointerEventType {
        None = 0,
        Enter = 1 << 0,
        Leave = 1 << 1,
        Motion = 1 << 2,
        Button = 1 << 3,
        Axis = 1 << 4,
        AxisSource = 1 << 5,
        AxisStop = 1 << 6,
        AxisDiscrete = 1 << 7,
        Axis120 = 1 << 8,
        AxisRelativeDirection = 1 << 9,
    };

protected:
    struct WaylandPointerEvent {
        PointerEventType eventType = PointerEventType::None;
        wl_fixed_t surfaceX;
        wl_fixed_t surfaceY;
        Uint32 button;
        Uint32 state;
        Uint32 time;
        Uint32 serial;
        struct {
            bool valid;
            wl_fixed_t value;
            Int32 discrete;
            Int32 value120;
            Uint32 relativeDirection;
        } axes[2];
        Uint32 axisSource;
    };

    static void xdgSurfaceConfigure(void* data, xdg_surface* xdg_surface, Uint32 serial);

    static void xdgToplevelConfigure(void* data,
                                     xdg_toplevel* xdg_toplevel,
                                     Int32 width,
                                     Int32 height,
                                     wl_array* states);

    static void xdgToplevelClose(void* data, xdg_toplevel* toplevel);
    static void xdgWmBasePing(void* data, xdg_wm_base* xdg_wm_base, Uint32 serial);

    static void xdgToplevelConfigureBounds(void* data, xdg_toplevel* xdg_toplevel, Int32 width, Int32 height);

    static void xdgToplevelWmCapabilities(void* data, xdg_toplevel* xdg_toplevel, wl_array* capabilities);

    static void wlSeatCapabilities(void* data, struct wl_seat* wl_seat, Uint32 capabilities);
    static void wlSeatName(void* data, struct wl_seat* wl_seat, const char* name);

    // Pointer EVENTS:
    static void wlPointerEnter(void* data,
                               struct wl_pointer* wl_pointer,
                               Uint32 serial,
                               struct wl_surface* surface,
                               wl_fixed_t surface_x,
                               wl_fixed_t surface_y);

    static void wlPointerLeave(void* data, struct wl_pointer* wl_pointer, Uint32 serial, struct wl_surface* surface);

    static void wlPointerMotion(void* data,
                                struct wl_pointer* wl_pointer,
                                Uint32 time,
                                wl_fixed_t surface_x,
                                wl_fixed_t surface_y);

    static void wlPointerButton(void* data,
                                struct wl_pointer* wl_pointer,
                                Uint32 serial,
                                Uint32 time,
                                Uint32 button,
                                Uint32 state);

    static void wlPointerAxis(void* data, struct wl_pointer* wl_pointer, Uint32 time, Uint32 axis, wl_fixed_t value);

    static void wlPointerFrame(void* data, struct wl_pointer* wl_pointer);

    static void wlPointerAxisSource(void* data, struct wl_pointer* wl_pointer, Uint32 axis_source);

    static void wlPointerAxisStop(void* data, struct wl_pointer* wl_pointer, Uint32 time, Uint32 axis);

    static void wlPointerAxisDiscrete(void* data, struct wl_pointer* wl_pointer, Uint32 axis, Int32 discrete);

    static void wlPointerAxisValue120(void* data, struct wl_pointer* wl_pointer, Uint32 axis, Int32 value120);

    static void wlPointerAxisRelativeDirection(void* data,
                                               struct wl_pointer* wl_pointer,
                                               Uint32 axis,
                                               Uint32 direction);
    // POINTER EVENTS END
    // Keyboard Events:

    static void wlKeyboardKeymap(void* data, struct wl_keyboard* wl_keyboard, Uint32 format, Int32 fd, Uint32 size);

    static void wlKeyboardEnter(void* data,
                                struct wl_keyboard* wl_keyboard,
                                Uint32 serial,
                                struct wl_surface* surface,
                                struct wl_array* keys);

    static void wlKeyboardLeave(void* data, struct wl_keyboard* wl_keyboard, Uint32 serial, struct wl_surface* surface);

    static void wlKeyboardKey(void* data,
                              struct wl_keyboard* wl_keyboard,
                              Uint32 serial,
                              Uint32 time,
                              Uint32 key,
                              Uint32 state);

    static void wlKeyboardModifiers(void* data,
                                    struct wl_keyboard* wl_keyboard,
                                    Uint32 serial,
                                    Uint32 mods_depressed,
                                    Uint32 mods_latched,
                                    Uint32 mods_locked,
                                    Uint32 group);

    static void wlKeyboardRepeatInfo(void* data, struct wl_keyboard* wl_keyboard, Int32 rate, Int32 delay);

    // Keyboard Events END

    wl_surface* m_surface = nullptr;
    xdg_surface* m_xdgSurface = nullptr;
    xdg_toplevel* m_xdgToplevel = nullptr;
    zxdg_toplevel_decoration_v1* m_zxdgToplevelDecoration = nullptr;
    wl_pointer* m_pointer = nullptr;
    wl_keyboard* m_keyboard = nullptr;
    // wl_touch* m_touch = nullptr;
    xkb_state* m_xkbState = nullptr;
    xkb_context* m_xkbContext = nullptr;
    xkb_keymap* m_xkbKeymap = nullptr;
    WaylandPointerEvent m_pointerEvent;
    bool m_close = false;

    wl_compositor* m_waylandCompositor;
    xdg_wm_base* m_xdgWmBase;
    zxdg_decoration_manager_v1* m_zxdgDecorationManager;
    wl_seat* m_seat;
    int m_fd = -1;
    static const xdg_wm_base_listener s_xdgWmBaseListener;
    static const xdg_surface_listener s_xdgSurfaceListener;
    static const xdg_toplevel_listener s_xdgToplevelListener;
    static const wl_seat_listener s_wlSeatListener;

    static const wl_pointer_listener s_wlPointerListener;
    static const wl_keyboard_listener s_wlKeyboardListener;
    // static const wl_touch_listener s_wlTouchListener;
};
}// namespace HUH

HUH_ENUM_CLASS_EQUAL_OPERATOR(HUH::WaylandWindow::PointerEventType)
HUH_ENUM_BIT_OPERATORS(HUH::WaylandWindow::PointerEventType)
