#pragma once

#include <HUH/Window/prototypes/window_proto.h>
#include <HUH/dynamic_library.h>
#include <HUH/types.h>
#include <string>

namespace HUH {
class HUH_WINDOW_API Window : public WindowProto {
public:
    Window(const std::string& name, HUH::Vector2u32 size);
    ~Window() override;

    using CreateWindowImpl = WindowProto*(const std::string&, const HUH::Vector2u32&);
    inline static CreateWindowImpl* s_createImpl = nullptr;
    HUH_NODISCARD const PlatformVariables& GetPlatformVariables() const override;
    void Show() override;
    bool Loop() override;

private:
    WindowProto* m_impl = nullptr;
    static DynamicLibrary s_lib;
};
}// namespace HUH