#pragma once
#include <HUH/definitions.h>
#include <string>
#include <utility>

typedef struct HWND__* HWND;
struct wl_display;
struct wl_surface;

namespace HUH {
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
    virtual void Loop() = 0;
    virtual ~WindowProto() = default;

    HUH_NODISCARD const PlatformVariables& GetPlatformVariables() const { return m_platform; }

protected:
    std::string m_name;
    Int32 m_width{};
    Int32 m_height{};
    PlatformVariables m_platform{};
};

}// namespace HUH