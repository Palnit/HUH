#include <HUH/Window/definitions.h>
#include <HUH/Windows/win_instance.h>
#include <iostream>
#include <HUH/Window/Windows/window.h>

namespace HUH {

KeyBindings TransformToHUHKey(WPARAM wParam) {
    switch (wParam) {
        case VK_LBUTTON:
            return KeyBindings::Mouse_1;
            case VK_RBUTTON:
            return KeyBindings::Mouse_2;
            case VK_MBUTTON:
            return KeyBindings::Mouse_3;
            case VK_XBUTTON1:
            return KeyBindings::Mouse_4;
            case VK_XBUTTON2:
            return KeyBindings::Mouse_5;
        default:
            return KeyBindings::Unknown;
    }
}

Window::Window(const std::string& name, const Int32 width, const Int32 height) : WindowProto(name, width, height) {
    constexpr char className[] = "HUH_WINDOWS_WINDOW";
    WNDCLASS wc = {};
    wc.lpfnWndProc = HUH::WindowProcPassToClass;
    wc.hInstance = HUH::g_AppInstance;
    wc.lpszClassName = className;

    RegisterClass(&wc);

    m_windowHandle = CreateWindowEx(0, className, m_name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                    m_width, m_height, nullptr, nullptr, HUH::g_AppInstance, nullptr);
    if (!m_windowHandle) {
        HUH_ILOG(LogWindow, "Window creation failed");
        return;
    }
    SetWindowLongPtr(m_windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    m_platform.WindowsHandle = m_windowHandle;
}

void Window::Show() {
    std::cout << "Hello World!" << std::endl;
    ShowWindow(m_windowHandle, HUH::g_CmdShow);
}
bool Window::Loop() {
    MSG msg = {};
    // peak message
    auto value = GetMessage(&msg, m_windowHandle, 0, 0);
    if (value > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return value > 0;
}

LRESULT Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (m_windowHandle != hwnd) {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    switch (uMsg) {
        case WM_CLOSE: {
            OnClose.ExecuteAll(this);
            if (MessageBox(m_windowHandle, "Really quit?", "My application", MB_OKCANCEL) == IDOK) {
                HUH_ILOG(LogWindow, "Closing Window");
                DestroyWindow(m_windowHandle);
            }
            return 0;
        }
        case WM_SIZE: {
            OnSizeChange.ExecuteAll(this, LOWORD(lParam), HIWORD(lParam));
            return 0;
        }
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN: {
            HUH_TLOG("Keydown?");
            KeyBindings key = TransformToHUHKey(wParam);
            HUH_TLOG("Key: {}",key);
            return 0;
        }
        case WM_MOUSEACTIVATE: {
            HUH_TLOG("Mouse activate");
            return MA_ACTIVATE;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

Window::~Window() {
    if (IsWindow(m_windowHandle)) {
        DestroyWindow(m_windowHandle);
    }
}

LRESULT WindowProcPassToClass(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (window) {
        return window->WindowProc(hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
}// namespace HUH