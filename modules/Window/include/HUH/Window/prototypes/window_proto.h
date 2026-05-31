#pragma once

#include <HUH/Math/vector.h>

#include <HUH/definitions.h>
#include <HUH/enum_define.h>
#include <HUH/event.h>

#include <string>

typedef struct HWND__* HWND;
struct wl_display;
struct wl_surface;

namespace HUH {
class Window;

enum class KeyBindings {
    Unknown,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    a,
    b,
    c,
    d,
    e,
    f,
    g,
    h,
    i,
    j,
    k,
    l,
    m,
    n,
    o,
    p,
    q,
    r,
    s,
    t,
    u,
    v,
    w,
    x,
    y,
    z,
    N0,
    N1,
    N2,
    N3,
    N4,
    N5,
    N6,
    N7,
    N8,
    N9,
    KP0,
    KP1,
    KP2,
    KP3,
    KP4,
    KP5,
    KP6,
    KP7,
    KP8,
    KP9,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,
    Mouse_1,
    Mouse_2,
    Mouse_3,
    Mouse_4,
    Mouse_5,
    ArrowLeft,
    ArrowRight,
    ArrowUp,
    ArrowDown,
    Escape,
    LeftControl,
    RightControl,
    LeftShift,
    RightShift,
    LeftAlt,
    RightAlt,
    BackSpace,
    Space,
    Enter,
    Tab,
    Other,
};

class HUH_WINDOW_API WindowProto {
public:
    struct PlatformVariables {
        HWND WindowsHandle;
        wl_display* WaylandDisplay;
        wl_surface* WaylandSurface;
    };
    WindowProto(const std::string& name, const HUH::Vector2u32 size) : m_name(name), m_size(size) {}
    virtual void Show() = 0;
    virtual bool Loop() = 0;
    virtual ~WindowProto() = default;

    HUH_NODISCARD HUH_CONSTEXPR_FORCE HUH::Vector2u32 GetSize() const { return m_size; }

    MultiEvent<void(Window*)> OnClose;
    MultiEvent<void(Window*, HUH::Vector2u32)> OnSizeChange;

    MultiEvent<void(Window*, HUH::Vector2d)> OnMouseEnter;
    MultiEvent<void(Window*)> OnMouseLeave;
    MultiEvent<void(Window*, HUH::Vector2d)> OnMouseMove;
    MultiEvent<void(Window*, KeyBindings)> OnMousePress;
    MultiEvent<void(Window*, KeyBindings)> OnMouseRelease;

    MultiEvent<void(Window*, KeyBindings, const std::string&)> OnKeyPress;
    MultiEvent<void(Window*, KeyBindings)> OnKeyRelease;

    HUH_NODISCARD virtual const PlatformVariables& GetPlatformVariables() const { return m_platform; }

protected:
    std::string m_name;
    HUH::Vector2u32 m_size;
    PlatformVariables m_platform{};
};

HUH_WINDOW_API std::string ToString(KeyBindings key);

}// namespace HUH

HUH_ENUM_FORMATER(HUH::KeyBindings)