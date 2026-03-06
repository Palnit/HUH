#pragma once

#include <HUH/types.h>
#include <HUH/dynamic_library.h>
#include <HUH/Window/prototypes/window_proto.h>
#include <string>

namespace HUH {
class HUH_WINDOW_API Window : public WindowProto {
public:
    Window(const std::string& name, const Int32 width, const Int32 height);
    ~Window() override;

    using CreateWindowImpl = WindowProto*(const std::string&, const Int32, const Int32);
    inline static CreateWindowImpl* s_createImpl = nullptr;
    HUH_NODISCARD const PlatformVariables& GetPlatformVariables() const override;
    void Show() override;
    bool Loop() override;

private:
    WindowProto* m_impl = nullptr;
    static DynamicLibrary s_lib;
};
}// namespace HUH