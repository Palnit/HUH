#pragma once
#include <HUH/Math/vector.h>

namespace HUH {
class Image {
public:
    Image() : Size(0, 0), Pixels() {}

    HUH::Vector2u32 Size;
    // TODO will do for now but should be replaced with my own with views instead of set u8 pixels
    std::vector<HUH::Vector4u8> Pixels;
};
}// namespace HUH
