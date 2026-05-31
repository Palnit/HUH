#pragma once

#include <HUH/Graphics/camera.h>
#include <HUH/Graphics/time.h>

#include <HUH/Window/window.h>

namespace HUH::Graphics {
class HUH_GRAPHICS_CAMERA_API CameraWindowConnector {
public:
    CameraWindowConnector(Camera& camera, Window& window, Time& time);
    ~CameraWindowConnector();

    KeyBindings Forward = KeyBindings::w;
    KeyBindings Backward = KeyBindings::s;
    KeyBindings Right = KeyBindings::d;
    KeyBindings Left = KeyBindings::a;
    KeyBindings Up = KeyBindings::Space;
    KeyBindings Down = KeyBindings::LeftControl;

    KeyBindings CameraMove = KeyBindings::Mouse_1;

    float Speed = 10;
    float Sensitivity = 0.01;

private:
    void KeyPress(Window* window, KeyBindings key_bindings, const std::string& string);
    void MouseMove(Window* window, Vector<double, 2> vector);
    void Resize(Window* window, Vector<unsigned, 2> vector) const;
    Camera& m_camera;
    Window& m_window;
    Time& m_time;
    bool m_mouseMove = false;
    Vector<double, 2> m_previousMousePos = {-1, -1};
    // TODO remake this to be with quaternion not separetly
    double yaw = 0;
    double pitch = 0;
    EventHandler m_keyPressedEventHandler;
    EventHandler m_keyReleaseEventHandler;
    EventHandler m_mousePressEventHandler;
    EventHandler m_mouseReleaseEventHandler;
    EventHandler m_mouseMoveEventHandler;
    EventHandler m_resizeEventHandler;
};
}// namespace HUH::Graphics