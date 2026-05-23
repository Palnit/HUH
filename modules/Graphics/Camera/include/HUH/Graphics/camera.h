#pragma once

#include "HUH/Graphics/transform.h"

#include <HUH/definitions.h>

namespace HUH::Graphics {
class HUH_GRAPHICS_CAMERA_API Camera {
public:
    Transform Transform;

    float NearClipPlane;
    float FarClipPlane;
};
}// namespace HUH::Graphics