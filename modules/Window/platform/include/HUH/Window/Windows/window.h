#pragma once

#include <HUH/Window/prototypes/window_proto.h>

namespace HUH {
class HUH_WINDOW_API Window : public WindowProto {
public:
    void Show() override;
    ~Window() override = default;

private:
};

}// namespace HUH
