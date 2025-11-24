#include <HUH/Window/definitions.h>
#include <HUH/Windows/win_instance.h>
#include <iostream>
#include <HUH/Window/Windows/window.h>

namespace HUH {
Window::Window(const std::string& name) : WindowProto(name) {
    constexpr char className[] = "HUH_WINDOWS_WINDOW";
    WNDCLASS wc = {};
    wc.lpfnWndProc = HUH::WindowProcPassToClass;
    wc.hInstance = HUH::g_AppInstance;
    wc.lpszClassName = className;

    RegisterClass(&wc);

    m_windowHandle = CreateWindowEx(0, className, m_name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                    CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, HUH::g_AppInstance, nullptr);
    if (!m_windowHandle) {
        HUH_ILOG(LogWindow, "Window creation failed");
        return;
    }
    SetWindowLongPtr(m_windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

void Window::Show() {
    std::cout << "Hello World!" << std::endl;
    ShowWindow(m_windowHandle, HUH::g_CmdShow);
}
void Window::Loop() {
    MSG msg = {};
    // peak message
    while (GetMessage(&msg, m_windowHandle, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (m_windowHandle != hwnd) {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    switch (uMsg) {
        case WM_CLOSE: {
            if (MessageBox(m_windowHandle, "Really quit?", "My application", MB_OKCANCEL) == IDOK) {
                HUH_ILOG(LogWindow, "Closing Window");
                DestroyWindow(m_windowHandle);
            }
            return 0;
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