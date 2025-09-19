#include <HUH/Graphics/Vulkan/basic_window.h>
#include <HUH/Graphics/generic_structs.h>
#include <imgui_impl_vulkan.h>

namespace HUH::Graphics::Vulkan{
BasicWindow::BasicWindow(const char* title, int x, int y, int width, int height, uint32_t flags)
    : m_title(title),
      m_x(x),
      m_y(y),
      m_width(width),
      m_height(height),
      m_flags(flags),
      m_running(true) {
    SDL_SetLogPriority(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR);
}

int BasicWindow::run() {
    if (InitWindow()) { return 1; }
    if (Init()) { return 1; }
    double fpsCount = 0;
    Uint64 fpsLastTime = SDL_GetTicks();
    ImGuiIO& io = ImGui::GetIO();
    while (m_running) {
        Time::DeltaTime = SDL_GetTicks() - Time::ElapsedTime;
        Time::ElapsedTime = SDL_GetTicks();
        double diff =
            static_cast<double>(Time::ElapsedTime - fpsLastTime) * 0.001;
        fpsCount++;
        if (diff >= 1.0 / 30.0) {
            Time::FPS = (1.0 / diff) * fpsCount;
            Time::Ms = (diff / fpsCount) * 1000;
            fpsLastTime = Time::ElapsedTime;
            fpsCount = 0;
        }
        while (SDL_PollEvent(&m_ev)) {

            bool isMouseCaptured = io.WantCaptureMouse;
            bool isKeyboardCaptured = io.WantCaptureKeyboard;

            switch (m_ev.type) {
                case SDL_EVENT_QUIT:
                    m_running = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (!isKeyboardCaptured) KeyboardDown(m_ev.key);
                    break;
                case SDL_EVENT_KEY_UP:
                    if (!isKeyboardCaptured) KeyboardUp(m_ev.key);
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (!isMouseCaptured) MouseDown(m_ev.button);
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (!isMouseCaptured) MouseUp(m_ev.button);
                    break;
                case SDL_EVENT_MOUSE_WHEEL:
                    if (!isMouseCaptured) MouseWheel(m_ev.wheel);
                    break;
                case SDL_EVENT_MOUSE_MOTION:
                    if (!isMouseCaptured) MouseMove(m_ev.motion);
                    break;
                case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                case SDL_EVENT_WINDOW_SHOWN:
                    SDL_GetWindowSize(m_window, &m_width, &m_height);
                    Resize();
                    break;
            }
        }
        Update();
        Render();


        ImGui::NewFrame();

        RenderImGui();

        ImGui::Render();
    }
    return 0;
}
BasicWindow::~BasicWindow() {
}

int BasicWindow::InitWindow() {


    return 0;
}

}
