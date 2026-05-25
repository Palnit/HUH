#pragma once

#include "HUH/Graphics/transform.h"

#include <HUH/definitions.h>

namespace HUH::Graphics {
class HUH_GRAPHICS_CAMERA_API Camera {
public:
    HUH::Graphics::Transform Transform;

    float NearClipPlane;
    float FarClipPlane;
    float Fov;
};
}// namespace HUH::Graphics