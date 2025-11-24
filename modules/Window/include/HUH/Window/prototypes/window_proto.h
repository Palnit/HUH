#pragma once
#include <HUH/definitions.h>

namespace HUH {
class HUH_WINDOW_API WindowProto {
public:
    virtual void Show() = 0;
    virtual ~WindowProto() = default;

private:
};

}// namespace HUH