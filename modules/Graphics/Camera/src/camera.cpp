#include <HUH/Graphics/camera.h>

#include "HUH/logging.h"

namespace HUH::Graphics {

void Camera::UpdateAspectRatio(HUH::Vector2u32 size) {
    AspectRatio = static_cast<float>(size.Width()) / static_cast<float>(size.Height());
}

Matrix4x4f Camera::GetViewMatrix() const {
    auto Matrix = Matrix4x4f({1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0},
                             {-Transform.Position.X(), -Transform.Position.Y(), -Transform.Position.Z(), 1});
    Matrix *= (-Transform.Rotation).ToMatrix();
    // Matrix[3][0] = -Transform.Position.X();
    // Matrix[3][1] = -Transform.Position.Y();
    // Matrix[3][2] = -Transform.Position.Z();
    Matrix *= Matrix4x4f{{0, 0, 1, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 1}};
    return Matrix;
}

Matrix4x4f Camera::GetPerspectiveProjectionMatrix() const {
    return HUH::GetPerspectiveMatrix(Fov, AspectRatio, NearClipPlane, FarClipPlane);
}

}// namespace HUH::Graphics