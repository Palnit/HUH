#pragma once

#include "HUH/Graphics/transform.h"
#include "HUH/Math/functions.h"

#include <HUH/definitions.h>

namespace HUH::Graphics {
class HUH_GRAPHICS_CAMERA_API Camera {
public:
    HUH::Graphics::Transform Transform;

    float NearClipPlane = 0.1;
    float FarClipPlane = 1000;
    float AspectRatio;
    float Fov = HUH::DegToRad(90);

    void UpdateAspectRatio(HUH::Vector2u32 size);
    Matrix4x4f GetViewMatrix() const;
    Matrix4x4f GetPerspectiveProjectionMatrix() const;
};
}// namespace HUH::Graphics