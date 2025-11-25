#pragma once
#include <HUH/definitions.h>

typedef struct HWND__* HWND;

namespace HUH {
class HUH_WINDOW_API WindowProto {
public:
    struct PlatformVariables {
        HWND WindowsHandle;
    };
    WindowProto(const std::string& name) : m_name(name) {}
    virtual void Show() = 0;
    virtual void Loop() = 0;
    virtual ~WindowProto() = default;

    HUH_NODISCARD const PlatformVariables& GetPlatformVariables() const { return m_platform; }

protected:
    std::string m_name;
    PlatformVariables m_platform;
};

}// namespace HUH