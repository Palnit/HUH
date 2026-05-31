#include <HUH/Graphics/camera_window_connector.h>

namespace HUH::Graphics {
CameraWindowConnector::CameraWindowConnector(Camera& camera, Window& window, Time& time)
    : m_camera(camera),
      m_window(window),
      m_time(time) {
    m_keyPressedEventHandler = m_window.OnKeyPress.Add(this, &CameraWindowConnector::KeyPress);
    m_resizeEventHandler = m_window.OnSizeChange.Add(this, &CameraWindowConnector::Resize);
    m_mousePressEventHandler = m_window.OnMousePress.Add([&](Window*, KeyBindings key) {
        if (key == CameraMove) {
            this->m_mouseMove = true;
        }
    });
    m_mouseReleaseEventHandler = m_window.OnMouseRelease.Add([&](Window*, KeyBindings key) {
        if (key == CameraMove) {
            this->m_mouseMove = false;
        }
    });
    m_keyReleaseEventHandler = m_window.OnKeyRelease.Add([&](Window*, KeyBindings key) {
        if (key == CameraMove) {
            this->m_mouseMove = false;
        }
    });
    m_mouseMoveEventHandler = m_window.OnMouseMove.Add(this, &CameraWindowConnector::MouseMove);
}

CameraWindowConnector::~CameraWindowConnector() {
    m_window.OnKeyPress.Remove(m_keyPressedEventHandler);
}

void CameraWindowConnector::KeyPress(Window*, KeyBindings key_bindings, const std::string&) {
    if (key_bindings == Forward) {
        m_camera.Transform.Position +=
            (-m_camera.Transform.Rotation).RotateVector(Vector3f(1, 0, 0)) * Speed * m_time.DeltaTime;
    }
    if (key_bindings == Backward) {
        m_camera.Transform.Position -=
            (-m_camera.Transform.Rotation).RotateVector(Vector3f(1, 0, 0)) * Speed * m_time.DeltaTime;
    }
    if (key_bindings == Right) {
        m_camera.Transform.Position +=
            (-m_camera.Transform.Rotation).RotateVector(Vector3f(0, 1, 0)) * Speed * m_time.DeltaTime;
    }
    if (key_bindings == Left) {
        m_camera.Transform.Position -=
            (-m_camera.Transform.Rotation).RotateVector(Vector3f(0, 1, 0)) * Speed * m_time.DeltaTime;
    }
    if (key_bindings == Up) {
        m_camera.Transform.Position +=
            (-m_camera.Transform.Rotation).RotateVector(Vector3f(0, 0, 1)) * Speed * m_time.DeltaTime;
    }
    if (key_bindings == Down) {
        m_camera.Transform.Position -=
            (-m_camera.Transform.Rotation).RotateVector(Vector3f(0, 0, 1)) * Speed * m_time.DeltaTime;
    }
    if (key_bindings == CameraMove) {
        m_mouseMove = true;
    }
}

void CameraWindowConnector::MouseMove(Window* window, Vector<double, 2> vector) {
    if (!m_mouseMove) {
        return;
    }
    if (m_previousMousePos == Vector<double, 2>{-1, -1}) {
        m_previousMousePos = vector;
        return;
    }

    auto delta = vector - m_previousMousePos;
    m_previousMousePos = vector;
    yaw -= delta.X() * Sensitivity;
    pitch -= delta.Y() * Sensitivity;
    pitch = HUH::Clamp(pitch, HUH::DegToRad(-89), HUH::DegToRad(89));
    m_camera.Transform.Rotation = Quaternion<float>(yaw, Vector3f(0, 0, 1));
    m_camera.Transform.Rotation.Rotate(pitch, Vector3f(0, 1, 0));
}

void CameraWindowConnector::Resize(Window* window, Vector<unsigned, 2> vector) const {
    m_camera.UpdateAspectRatio(vector);
}

}// namespace HUH::Graphics
// namespace HUH::Graphics
