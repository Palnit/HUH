#pragma once

#include <HUH/definitions.h>
#include <HUH/Window/prototypes/window_proto.h>

namespace HUH {

extern "C" WindowProto* CreateWindowImpl(const std::string& name);

class HUH_WAYLANDWINDOW_API WaylandWindow : public WindowProto {
public:
    WaylandWindow(const std::string& name);
    friend WindowProto* CreateWindowImpl(const std::string& name);
    void Show() override;
    void Loop() override;
};
}// namespace HUH
