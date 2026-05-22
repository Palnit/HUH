#pragma once

#include <windows.h>
#include <HUH/Window/prototypes/window_proto.h>

namespace HUH {

LRESULT CALLBACK WindowProcPassToClass(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
class HUH_WINDOW_API Window : public WindowProto {
public:
    Window(const std::string& name, HUH::Vector2u32 size);
    ~Window() override;
    void Show() override;
    bool Loop() override;

private:
    friend LRESULT CALLBACK WindowProcPassToClass(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND m_windowHandle = nullptr;
};

}// namespace HUH
