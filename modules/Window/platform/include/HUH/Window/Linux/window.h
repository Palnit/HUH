#pragma once

#include <HUH/Window/prototypes/window_proto.h>
#include <string>

namespace HUH {

class HUH_WINDOW_API Window : public WindowProto {
public:
    Window(const std::string& name);
    void Show() override;
    void Loop() override;

private:
    WindowProto* m_impl = nullptr;
};
}// namespace HUH