#pragma once
#include <HUH/definitions.h>

namespace HUH {
class HUH_WINDOW_API WindowProto {
public:
    WindowProto(const std::string& name) : m_name(name) {}
    virtual void Show() = 0;
    virtual void Loop() = 0;
    virtual ~WindowProto() = default;

protected:
    std::string m_name;
};

}// namespace HUH