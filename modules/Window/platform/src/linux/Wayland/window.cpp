#include <HUH/Window/Linux/Wayland/window.h>

#include <HUH/Window/Linux/Wayland/definitions.h>
#include <HUH/enum_helper.h>
#include <HUH/wayland_connector.h>

#include <linux/input-event-codes.h>
#include <poll.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>

#include <wayland-client.h>
#include <wayland-protocols/xdg-decoration-client-protocol.h>
#include <wayland-protocols/xdg-shell-client-protocol.h>

#include <xkbcommon/xkbcommon.h>

namespace HUH {

const xdg_wm_base_listener WaylandWindow::s_xdgWmBaseListener = {.ping = WaylandWindow::xdgWmBasePing};
const xdg_surface_listener WaylandWindow::s_xdgSurfaceListener = {.configure = WaylandWindow::xdgSurfaceConfigure};
const xdg_toplevel_listener WaylandWindow::s_xdgToplevelListener = {
    .configure = WaylandWindow::xdgToplevelConfigure,
    .close = WaylandWindow::xdgToplevelClose,
    .configure_bounds = WaylandWindow::xdgToplevelConfigureBounds,
    .wm_capabilities = WaylandWindow::xdgToplevelWmCapabilities,
};
const wl_seat_listener WaylandWindow::s_wlSeatListener = {.capabilities = WaylandWindow::wlSeatCapabilities,
                                                          .name = WaylandWindow::wlSeatName};

const wl_pointer_listener WaylandWindow::s_wlPointerListener = {
    .enter = WaylandWindow::wlPointerEnter,
    .leave = WaylandWindow::wlPointerLeave,
    .motion = WaylandWindow::wlPointerMotion,
    .button = WaylandWindow::wlPointerButton,
    .axis = WaylandWindow::wlPointerAxis,
    .frame = WaylandWindow::wlPointerFrame,
    .axis_source = WaylandWindow::wlPointerAxisSource,
    .axis_stop = WaylandWindow::wlPointerAxisStop,
    .axis_discrete = WaylandWindow::wlPointerAxisDiscrete,
    .axis_value120 = WaylandWindow::wlPointerAxisValue120,
    .axis_relative_direction = WaylandWindow::wlPointerAxisRelativeDirection};

const wl_keyboard_listener WaylandWindow::s_wlKeyboardListener = {.keymap = WaylandWindow::wlKeyboardKeymap,
                                                                  .enter = WaylandWindow::wlKeyboardEnter,
                                                                  .leave = WaylandWindow::wlKeyboardLeave,
                                                                  .key = WaylandWindow::wlKeyboardKey,
                                                                  .modifiers = WaylandWindow::wlKeyboardModifiers,
                                                                  .repeat_info = WaylandWindow::wlKeyboardRepeatInfo};
// const wl_touch_listener WaylandWindow::s_wlTouchListener = {};

extern "C" WindowProto* CreateWindowImpl(const std::string& name, const HUH::Vector2u32& size) {
    return new WaylandWindow(name, size);
}

KeyBindings XkbToKeyBindings(xkb_keysym_t keysym) {
    switch (keysym) {
        case XKB_KEY_A:
            return KeyBindings::A;
        case XKB_KEY_B:
            return KeyBindings::B;
        case XKB_KEY_C:
            return KeyBindings::C;
        case XKB_KEY_D:
            return KeyBindings::D;
        case XKB_KEY_E:
            return KeyBindings::E;
        case XKB_KEY_F:
            return KeyBindings::F;
        case XKB_KEY_G:
            return KeyBindings::G;
        case XKB_KEY_H:
            return KeyBindings::H;
        case XKB_KEY_I:
            return KeyBindings::I;
        case XKB_KEY_J:
            return KeyBindings::J;
        case XKB_KEY_K:
            return KeyBindings::K;
        case XKB_KEY_L:
            return KeyBindings::L;
        case XKB_KEY_M:
            return KeyBindings::M;
        case XKB_KEY_N:
            return KeyBindings::N;
        case XKB_KEY_O:
            return KeyBindings::O;
        case XKB_KEY_P:
            return KeyBindings::P;
        case XKB_KEY_Q:
            return KeyBindings::Q;
        case XKB_KEY_R:
            return KeyBindings::R;
        case XKB_KEY_S:
            return KeyBindings::S;
        case XKB_KEY_T:
            return KeyBindings::T;
        case XKB_KEY_U:
            return KeyBindings::U;
        case XKB_KEY_V:
            return KeyBindings::V;
        case XKB_KEY_W:
            return KeyBindings::W;
        case XKB_KEY_X:
            return KeyBindings::X;
        case XKB_KEY_Y:
            return KeyBindings::Y;
        case XKB_KEY_Z:
            return KeyBindings::Z;
        case XKB_KEY_a:
            return KeyBindings::a;
        case XKB_KEY_b:
            return KeyBindings::b;
        case XKB_KEY_c:
            return KeyBindings::c;
        case XKB_KEY_d:
            return KeyBindings::d;
        case XKB_KEY_e:
            return KeyBindings::e;
        case XKB_KEY_f:
            return KeyBindings::f;
        case XKB_KEY_g:
            return KeyBindings::g;
        case XKB_KEY_h:
            return KeyBindings::h;
        case XKB_KEY_i:
            return KeyBindings::i;
        case XKB_KEY_j:
            return KeyBindings::j;
        case XKB_KEY_k:
            return KeyBindings::k;
        case XKB_KEY_l:
            return KeyBindings::l;
        case XKB_KEY_m:
            return KeyBindings::m;
        case XKB_KEY_n:
            return KeyBindings::n;
        case XKB_KEY_o:
            return KeyBindings::o;
        case XKB_KEY_p:
            return KeyBindings::p;
        case XKB_KEY_q:
            return KeyBindings::q;
        case XKB_KEY_r:
            return KeyBindings::r;
        case XKB_KEY_s:
            return KeyBindings::s;
        case XKB_KEY_t:
            return KeyBindings::t;
        case XKB_KEY_u:
            return KeyBindings::u;
        case XKB_KEY_v:
            return KeyBindings::v;
        case XKB_KEY_w:
            return KeyBindings::w;
        case XKB_KEY_x:
            return KeyBindings::x;
        case XKB_KEY_y:
            return KeyBindings::y;
        case XKB_KEY_z:
            return KeyBindings::z;
        case XKB_KEY_0:
            return KeyBindings::N0;
        case XKB_KEY_1:
            return KeyBindings::N1;
        case XKB_KEY_2:
            return KeyBindings::N2;
        case XKB_KEY_3:
            return KeyBindings::N3;
        case XKB_KEY_4:
            return KeyBindings::N4;
        case XKB_KEY_5:
            return KeyBindings::N5;
        case XKB_KEY_6:
            return KeyBindings::N6;
        case XKB_KEY_7:
            return KeyBindings::N7;
        case XKB_KEY_8:
            return KeyBindings::N8;
        case XKB_KEY_9:
            return KeyBindings::N9;
        case XKB_KEY_KP_0:
            return KeyBindings::KP0;
        case XKB_KEY_KP_1:
            return KeyBindings::KP1;
        case XKB_KEY_KP_2:
            return KeyBindings::KP2;
        case XKB_KEY_KP_3:
            return KeyBindings::KP3;
        case XKB_KEY_KP_4:
            return KeyBindings::KP4;
        case XKB_KEY_KP_5:
            return KeyBindings::KP5;
        case XKB_KEY_KP_6:
            return KeyBindings::KP6;
        case XKB_KEY_KP_7:
            return KeyBindings::KP7;
        case XKB_KEY_KP_8:
            return KeyBindings::KP8;
        case XKB_KEY_KP_9:
            return KeyBindings::KP9;
        case XKB_KEY_F1:
            return KeyBindings::F1;
        case XKB_KEY_F2:
            return KeyBindings::F2;
        case XKB_KEY_F3:
            return KeyBindings::F3;
        case XKB_KEY_F4:
            return KeyBindings::F4;
        case XKB_KEY_F5:
            return KeyBindings::F5;
        case XKB_KEY_F6:
            return KeyBindings::F6;
        case XKB_KEY_F7:
            return KeyBindings::F7;
        case XKB_KEY_F8:
            return KeyBindings::F8;
        case XKB_KEY_F9:
            return KeyBindings::F9;
        case XKB_KEY_F10:
            return KeyBindings::F10;
        case XKB_KEY_F11:
            return KeyBindings::F11;
        case XKB_KEY_F12:
            return KeyBindings::F12;
        case XKB_KEY_F13:
            return KeyBindings::F13;
        case XKB_KEY_F14:
            return KeyBindings::F14;
        case XKB_KEY_F15:
            return KeyBindings::F15;
        case XKB_KEY_F16:
            return KeyBindings::F16;
        case XKB_KEY_F17:
            return KeyBindings::F17;
        case XKB_KEY_F18:
            return KeyBindings::F18;
        case XKB_KEY_F19:
            return KeyBindings::F19;
        case XKB_KEY_F20:
            return KeyBindings::F20;
        case XKB_KEY_F21:
            return KeyBindings::F21;
        case XKB_KEY_F22:
            return KeyBindings::F22;
        case XKB_KEY_F23:
            return KeyBindings::F23;
        case XKB_KEY_F24:
            return KeyBindings::F24;
        case XKB_KEY_Up:
            return KeyBindings::ArrowUp;
        case XKB_KEY_Down:
            return KeyBindings::ArrowDown;
        case XKB_KEY_Left:
            return KeyBindings::ArrowLeft;
        case XKB_KEY_Right:
            return KeyBindings::ArrowRight;
        case XKB_KEY_Escape:
            return KeyBindings::Escape;
        case XKB_KEY_Control_L:
            return KeyBindings::LeftControl;
        case XKB_KEY_Control_R:
            return KeyBindings::RightControl;
        case XKB_KEY_Shift_L:
            return KeyBindings::LeftShift;
        case XKB_KEY_Shift_R:
            return KeyBindings::RightShift;
        case XKB_KEY_Alt_L:
            return KeyBindings::LeftAlt;
        case XKB_KEY_Alt_R:
            return KeyBindings::RightAlt;
        case XKB_KEY_BackSpace:
            return KeyBindings::BackSpace;
        case XKB_KEY_Tab:
            return KeyBindings::Tab;
        case XKB_KEY_space:
            return KeyBindings::Space;
        case XKB_KEY_KP_Enter:
        case XKB_KEY_ISO_Enter:
        case XKB_KEY_Return:
            return KeyBindings::Enter;

        default:
            return KeyBindings::Other;
    }
}

void WaylandWindow::xdgWmBasePing(void* data, xdg_wm_base* xdg_wm_base, const Uint32 serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

void WaylandWindow::xdgSurfaceConfigure(void* data, xdg_surface* xdg_surface, const Uint32 serial) {
    const auto* window = static_cast<WaylandWindow*>(data);
    xdg_surface_ack_configure(xdg_surface, serial);
    wl_surface_commit(window->m_surface);
}

void WaylandWindow::xdgToplevelConfigure(void* data,
                                         xdg_toplevel* xdg_toplevel,
                                         Int32 width,
                                         Int32 height,
                                         wl_array* states) {
    const auto window = static_cast<WaylandWindow*>(data);
    if (width == 0 || height == 0) {
        return;
    }
    window->m_size = {static_cast<Uint32>(width), static_cast<Uint32>(height)};
    window->OnSizeChange.ExecuteAll(nullptr, window->m_size);
}

void WaylandWindow::xdgToplevelClose(void* data, xdg_toplevel* toplevel) {
    const auto window = static_cast<WaylandWindow*>(data);
    window->OnClose.ExecuteAll(nullptr);
    window->m_close = true;
}

void WaylandWindow::xdgToplevelConfigureBounds(void* data, xdg_toplevel* xdg_toplevel, Int32 width, Int32 height) {
}

void WaylandWindow::xdgToplevelWmCapabilities(void* data, xdg_toplevel* xdg_toplevel, wl_array* capabilities) {
}

void WaylandWindow::wlSeatCapabilities(void* data, wl_seat* wl_seat, Uint32 capabilities) {
    const auto window = static_cast<WaylandWindow*>(data);
    if (HUH::CheckFlag(static_cast<wl_seat_capability>(capabilities), WL_SEAT_CAPABILITY_POINTER)
        && window->m_pointer == nullptr) {
        window->m_pointer = wl_seat_get_pointer(window->m_seat);
        wl_pointer_add_listener(window->m_pointer, &s_wlPointerListener, window);
    } else if (HUH::CheckFlag(static_cast<wl_seat_capability>(capabilities), WL_SEAT_CAPABILITY_POINTER)
               && window->m_pointer != nullptr) {
        wl_pointer_release(window->m_pointer);
        window->m_pointer = nullptr;
    }

    if (HUH::CheckFlag(static_cast<wl_seat_capability>(capabilities), WL_SEAT_CAPABILITY_KEYBOARD)
        && window->m_keyboard == nullptr) {
        window->m_keyboard = wl_seat_get_keyboard(window->m_seat);
        wl_keyboard_add_listener(window->m_keyboard, &s_wlKeyboardListener, window);
    } else if (HUH::CheckFlag(static_cast<wl_seat_capability>(capabilities), WL_SEAT_CAPABILITY_KEYBOARD)
               && window->m_keyboard != nullptr) {
        wl_keyboard_release(window->m_keyboard);
        window->m_keyboard = nullptr;
    }

    // if (HUH::CheckFlag(static_cast<wl_seat_capability>(capabilities), WL_SEAT_CAPABILITY_TOUCH)
    //     && window->m_touch == nullptr) {
    //     window->m_touch = wl_seat_get_touch(window->m_seat);
    //     wl_touch_add_listener(window->m_touch, &s_wlTouchListener, window);
    // } else if (HUH::CheckFlag(static_cast<wl_seat_capability>(capabilities), WL_SEAT_CAPABILITY_TOUCH)
    //            && window->m_touch != nullptr) {
    //     wl_touch_release(window->m_touch);
    //     window->m_touch = nullptr;
    // }
}

void WaylandWindow::wlSeatName(void* data, wl_seat* wl_seat, const char* name) {
    HUH_ILOG(LogWaylandWindow, "Name of Wayland Seat: {}", name)
}

void WaylandWindow::wlPointerEnter(void* data,
                                   wl_pointer* wl_pointer,
                                   Uint32 serial,
                                   wl_surface* surface,
                                   wl_fixed_t surface_x,
                                   wl_fixed_t surface_y) {
    const auto window = static_cast<WaylandWindow*>(data);
    window->m_pointerEvent.eventType |= PointerEventType::Enter;
    window->m_pointerEvent.serial = serial;
    window->m_pointerEvent.surfaceX = surface_x;
    window->m_pointerEvent.surfaceY = surface_y;
}

void WaylandWindow::wlPointerLeave(void* data, wl_pointer* wl_pointer, Uint32 serial, wl_surface* surface) {
    const auto window = static_cast<WaylandWindow*>(data);
    window->m_pointerEvent.eventType |= PointerEventType::Leave;
    window->m_pointerEvent.serial = serial;
}

void WaylandWindow::wlPointerMotion(void* data,
                                    wl_pointer* wl_pointer,
                                    Uint32 time,
                                    wl_fixed_t surface_x,
                                    wl_fixed_t surface_y) {

    const auto window = static_cast<WaylandWindow*>(data);
    window->m_pointerEvent.eventType |= PointerEventType::Motion;
    window->m_pointerEvent.time = time;
    window->m_pointerEvent.surfaceX = surface_x;
    window->m_pointerEvent.surfaceY = surface_y;
}
void WaylandWindow::wlPointerButton(void* data,
                                    wl_pointer* wl_pointer,
                                    Uint32 serial,
                                    Uint32 time,
                                    Uint32 button,
                                    Uint32 state) {

    const auto window = static_cast<WaylandWindow*>(data);
    window->m_pointerEvent.eventType |= PointerEventType::Button;
    window->m_pointerEvent.serial = serial;
    window->m_pointerEvent.time = time;
    window->m_pointerEvent.button = button;
    window->m_pointerEvent.state = state;
}
void WaylandWindow::wlPointerAxis(void* data, wl_pointer* wl_pointer, Uint32 time, Uint32 axis, wl_fixed_t value) {
    const auto window = static_cast<WaylandWindow*>(data);
    window->m_pointerEvent.eventType |= PointerEventType::Axis;
    window->m_pointerEvent.time = time;
    window->m_pointerEvent.axes[axis].valid = true;
    window->m_pointerEvent.axes[axis].value = value;
}

void WaylandWindow::wlPointerFrame(void* data, wl_pointer* wl_pointer) {
    const auto window = static_cast<WaylandWindow*>(data);
    auto& event = window->m_pointerEvent;
    if (HUH::CheckFlag(event.eventType, PointerEventType::Enter)) {
        window->OnMouseEnter.ExecuteAll(nullptr,
                                        {wl_fixed_to_double(event.surfaceX), wl_fixed_to_double(event.surfaceY)});
    }
    if (HUH::CheckAllFlag(event.eventType, PointerEventType::Leave)) {
        window->OnMouseLeave.ExecuteAll(nullptr);
    }
    if (HUH::CheckFlag(event.eventType, PointerEventType::Button)) {
        KeyBindings binding = KeyBindings::Unknown;
        if (event.button == BTN_LEFT) {
            binding = KeyBindings::Mouse_1;
        }
        if (event.button == BTN_RIGHT) {
            binding = KeyBindings::Mouse_2;
        }
        if (event.button == BTN_MIDDLE) {
            binding = KeyBindings::Mouse_3;
        }
        if (event.button == BTN_EXTRA) {
            binding = KeyBindings::Mouse_4;
        }
        if (event.button == BTN_SIDE) {
            binding = KeyBindings::Mouse_5;
        }
        if (event.state == WL_POINTER_BUTTON_STATE_RELEASED) {
            window->OnMouseRelease(nullptr, binding);
        } else {
            window->OnMousePress(nullptr, binding);
        }
    }
    if (HUH::CheckFlag(event.eventType, PointerEventType::Motion)) {
        window->OnMouseMove.ExecuteAll(nullptr,
                                       {wl_fixed_to_double(event.surfaceX), wl_fixed_to_double(event.surfaceY)});
    }
    // TODO AXIS

    event = {};
}

void WaylandWindow::wlPointerAxisSource(void* data, wl_pointer* wl_pointer, Uint32 axis_source) {
    const auto window = static_cast<WaylandWindow*>(data);
    window->m_pointerEvent.eventType |= PointerEventType::AxisSource;
    window->m_pointerEvent.axisSource = axis_source;
}

void WaylandWindow::wlPointerAxisStop(void* data, wl_pointer* wl_pointer, Uint32 time, Uint32 axis) {
    const auto window = static_cast<WaylandWindow*>(data);
    window->m_pointerEvent.eventType |= PointerEventType::AxisStop;
    window->m_pointerEvent.time = time;
    window->m_pointerEvent.axes[axis].valid = true;
}

void WaylandWindow::wlPointerAxisDiscrete(void* data, wl_pointer* wl_pointer, Uint32 axis, Int32 discrete) {
    const auto window = static_cast<WaylandWindow*>(data);
    window->m_pointerEvent.eventType |= PointerEventType::AxisDiscrete;
    window->m_pointerEvent.axes[axis].valid = true;
    window->m_pointerEvent.axes[axis].discrete = discrete;
}

void WaylandWindow::wlPointerAxisValue120(void* data, wl_pointer* wl_pointer, Uint32 axis, Int32 value120) {
    const auto window = static_cast<WaylandWindow*>(data);
    window->m_pointerEvent.eventType |= PointerEventType::Axis120;
    window->m_pointerEvent.axes[axis].valid = true;
    window->m_pointerEvent.axes[axis].value120 = value120;
}

void WaylandWindow::wlPointerAxisRelativeDirection(void* data, wl_pointer* wl_pointer, Uint32 axis, Uint32 direction) {
    const auto window = static_cast<WaylandWindow*>(data);
    window->m_pointerEvent.eventType |= PointerEventType::AxisRelativeDirection;
    window->m_pointerEvent.axes[axis].valid = true;
    window->m_pointerEvent.axes[axis].relativeDirection = direction;
}

void WaylandWindow::wlKeyboardKeymap(void* data, wl_keyboard* wl_keyboard, Uint32 format, Int32 fd, Uint32 size) {
    const auto window = static_cast<WaylandWindow*>(data);
    if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
        return;
    }

    const auto map_shm = static_cast<char*>(mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, 0));

    xkb_keymap* xkb_keymap = xkb_keymap_new_from_string(window->m_xkbContext, map_shm, XKB_KEYMAP_FORMAT_TEXT_V1,
                                                        XKB_KEYMAP_COMPILE_NO_FLAGS);
    munmap(map_shm, size);
    close(fd);

    xkb_state* xkb_state = xkb_state_new(xkb_keymap);
    xkb_keymap_unref(window->m_xkbKeymap);
    xkb_state_unref(window->m_xkbState);
    window->m_xkbKeymap = xkb_keymap;
    window->m_xkbState = xkb_state;
}
void WaylandWindow::wlKeyboardEnter(void* data,
                                    wl_keyboard* wl_keyboard,
                                    Uint32 serial,
                                    wl_surface* surface,
                                    wl_array* keys) {
    const auto window = static_cast<WaylandWindow*>(data);

    for (Uint32* key = static_cast<Uint32*>(keys->data);
         keys->size != 0 && reinterpret_cast<const char*>(key) < static_cast<const char*>(keys->data) + keys->size;
         key++) {
        xkb_keysym_t sym = xkb_state_key_get_one_sym(window->m_xkbState, *key + 8);
        auto keybinding = XkbToKeyBindings(sym);
        // char buf[128];
        // xkb_keysym_get_name(sym, buf, sizeof(buf));
        // HUH_TLOG("HUH?: {}", buf)
        auto size = xkb_state_key_get_utf8(window->m_xkbState, *key + 8, nullptr, 0);
        std::string utf8;
        utf8.resize(size);
        xkb_state_key_get_utf8(window->m_xkbState, *key + 8, utf8.data(), size + 1);
        window->OnKeyPress(nullptr, keybinding, utf8);
    }
}
void WaylandWindow::wlKeyboardLeave(void* data, wl_keyboard* wl_keyboard, Uint32 serial, wl_surface* surface) {
}

void WaylandWindow::wlKeyboardKey(void* data,
                                  wl_keyboard* wl_keyboard,
                                  Uint32 serial,
                                  Uint32 time,
                                  const Uint32 key,
                                  const Uint32 state) {
    const auto window = static_cast<WaylandWindow*>(data);
    Uint32 keycode = key + 8;
    xkb_keysym_t sym = xkb_state_key_get_one_sym(window->m_xkbState, keycode);
    auto keybinding = XkbToKeyBindings(sym);
    if (state == WL_KEYBOARD_KEY_STATE_PRESSED || state == WL_KEYBOARD_KEY_STATE_REPEATED) {
        auto size = xkb_state_key_get_utf8(window->m_xkbState, keycode, nullptr, 0);
        // char buf[128];
        // xkb_keysym_get_name(sym, buf, sizeof(buf));
        // HUH_TLOG("HUH?: {}", buf)
        std::string utf8;
        utf8.resize(size);
        xkb_state_key_get_utf8(window->m_xkbState, keycode, utf8.data(), size + 1);
        window->OnKeyPress(nullptr, keybinding, utf8);
        return;
    }
    window->OnKeyRelease(nullptr, keybinding);
}
void WaylandWindow::wlKeyboardModifiers(void* data,
                                        wl_keyboard* wl_keyboard,
                                        Uint32 serial,
                                        Uint32 mods_depressed,
                                        Uint32 mods_latched,
                                        Uint32 mods_locked,
                                        Uint32 group) {
    const auto window = static_cast<WaylandWindow*>(data);
    xkb_state_update_mask(window->m_xkbState, mods_depressed, mods_latched, mods_locked, 0, 0, group);
}
void WaylandWindow::wlKeyboardRepeatInfo(void* data, wl_keyboard* wl_keyboard, Int32 rate, Int32 delay) {
}

WaylandWindow::WaylandWindow(const std::string& name, HUH::Vector2u32 size) : WindowProto(name, size) {

    m_xkbContext = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    m_fd = wl_display_get_fd(HUH::g_waylandConnector);
    m_waylandCompositor = HUH::g_waylandConnector.RegisterGlobal<wl_compositor>(&wl_compositor_interface);

    m_xdgWmBase = HUH::g_waylandConnector.RegisterGlobal<xdg_wm_base>(&xdg_wm_base_interface);
    xdg_wm_base_add_listener(m_xdgWmBase, &s_xdgWmBaseListener, this);

    m_zxdgDecorationManager =
        HUH::g_waylandConnector.RegisterGlobal<zxdg_decoration_manager_v1>(&zxdg_decoration_manager_v1_interface);

    m_seat = HUH::g_waylandConnector.RegisterGlobal<wl_seat>(&wl_seat_interface);
    wl_seat_add_listener(m_seat, &s_wlSeatListener, this);

    m_surface = wl_compositor_create_surface(m_waylandCompositor);
    m_xdgSurface = xdg_wm_base_get_xdg_surface(m_xdgWmBase, m_surface);
    xdg_surface_add_listener(m_xdgSurface, &s_xdgSurfaceListener, this);

    m_xdgToplevel = xdg_surface_get_toplevel(m_xdgSurface);
    xdg_toplevel_add_listener(m_xdgToplevel, &s_xdgToplevelListener, this);

    m_zxdgToplevelDecoration =
        zxdg_decoration_manager_v1_get_toplevel_decoration(m_zxdgDecorationManager, m_xdgToplevel);
    zxdg_toplevel_decoration_v1_set_mode(m_zxdgToplevelDecoration, ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
    xdg_toplevel_set_min_size(m_xdgToplevel, size.X(), size.Y());
    // xdg_toplevel_set_max_size(m_xdgToplevel, m_width, m_height);
    xdg_toplevel_set_title(m_xdgToplevel, m_name.c_str());
    wl_surface_commit(m_surface);
    m_platform.WaylandDisplay = HUH::g_waylandConnector;
    m_platform.WaylandSurface = m_surface;
}

WaylandWindow::~WaylandWindow() {
    zxdg_toplevel_decoration_v1_destroy(m_zxdgToplevelDecoration);
    xdg_toplevel_destroy(m_xdgToplevel);
    xdg_surface_destroy(m_xdgSurface);
    wl_surface_destroy(m_surface);
}

void WaylandWindow::Show() {
    HUH_ILOG(LogWaylandWindow, "WaylandWindow::Show");
}

bool WaylandWindow::Loop() {
    pollfd pfd{
        .fd = m_fd,
        .events = POLLIN,
    };
    while (wl_display_prepare_read(HUH::g_waylandConnector) != 0) {
        if (wl_display_dispatch_pending(HUH::g_waylandConnector) == -1) {
            return false;
        }
    }
    while (wl_display_flush(HUH::g_waylandConnector) == -1) {
        if (errno == EAGAIN) {
            pfd.events = POLLOUT;
            poll(&pfd, 1, -1);
        } else {
            return false;
        }
    }
    pfd.events = POLLIN;
    poll(&pfd, 1, -1);

    bool value = wl_display_read_events(HUH::g_waylandConnector) == 0;
    value = value && m_close == false;
    value = value && wl_display_dispatch_pending(HUH::g_waylandConnector) != -1;

    return value;
}

}// namespace HUH