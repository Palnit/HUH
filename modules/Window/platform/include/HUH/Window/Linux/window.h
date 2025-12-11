#pragma once

#include <HUH/dynamic_library.h>
#include <HUH/Window/prototypes/window_proto.h>
#include <string>

namespace HUH {
class HUH_WINDOW_API Window : public WindowProto {
public:
    Window(const std::string& name);
    ~Window() override;

    using CreateWindowImpl = WindowProto*(const std::string&);
    inline static CreateWindowImpl* s_createImpl = nullptr;
    void Show() override;
    void Loop() override;

private:
    WindowProto* m_impl = nullptr;
    static DynamicLibrary s_lib;
};
}// namespace HUH