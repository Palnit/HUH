#pragma once

#include <HUH/event.h>
#include <HUH/definitions.h>
#include <string>

typedef struct HWND__* HWND;
struct wl_display;
struct wl_surface;

namespace HUH {
class Window;

class HUH_WINDOW_API WindowProto {
public:
    struct PlatformVariables {
        HWND WindowsHandle;
        wl_display* WaylandDisplay;
        wl_surface* WaylandSurface;
    };
    WindowProto(const std::string& name, const Int32 width, const Int32 height)
        : m_name(name),
          m_width(width),
          m_height(height) {}
    virtual void Show() = 0;
    virtual bool Loop() = 0;
    virtual ~WindowProto() = default;

    HUH_NODISCARD HUH_CONSTEXPR_FORCE Int32 GetWidth() const { return m_width; }
    HUH_NODISCARD HUH_CONSTEXPR_FORCE Int32 GetHeight() const { return m_height; }

    MultiEvent<void(Window*)> OnClose;
    MultiEvent<void(Window*, Int32 Width, Int32 Height)> OnSizeChange;

    HUH_NODISCARD virtual const PlatformVariables& GetPlatformVariables() const { return m_platform; }

protected:
    std::string m_name;
    Int32 m_width{};
    Int32 m_height{};
    PlatformVariables m_platform{};
};

}// namespace HUH